/*
 * Copyright (c) 2021, suncloudsmoon and the Bootstrapped Freeze Interpreter contributors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * interpreter.c
 *
 *  Created on: Jul 23, 2021
 *      Author: suncloudsmoon
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/interpreter.h"
#include "../include/stringobj.h"

/*
 * What needs to be defined in order to bootstrap:
 * - string functions (string declare, string add)
 * - goto to a line (so for loops can be implemented) and goto next function with specified name ("goto function: elseif"), so control flows can be implemented
 * - ability to call, declare methods
 * - ability to interpret one condition (10 > 5) - only numbers is enough
 * - ability to store ("store: 5, var")
 * - ability to change the grammar of the language (like the core functions itself defined in the vm_t structure)
 *
 * --- Very easy to implement --
 * - ability to print
 * - ability to read/write file
 * - ability to use system commands (like system())
 */

/*
 * Example if statement to be implemented inside the interpreted language:
 * (nothing can be named "and" or "or")
 * if: 10 > 5
 * 	print: "hello\n"
 * elseif: 5 > 10
 * 	print: "wacked\n"
 * else:
 * 	print: "math is crazy!\n"
 * end
 */

// Static Prototypes
static int readLine(string_t *str, FILE *stream);
static list_t* parse_split_offquotes(string_t *delimiter, string_t *line);

vm_t* vm_init() {
	vm_t *vm = malloc(sizeof(vm_t));

	// Extremely Core Components of the interpreter
	vm->set_delimiter = ' ';
	vm->arg_delimiter = ',';

	vm->var_declare = string_copyvalueof("set");
	vm->var_add = string_copyvalueof("add");

	vm->goto_line = string_copyvalueof("gotoline");
	vm->goto_function = string_copyvalueof("gotofunc");

	vm->function_declare = string_copyvalueof("function");
	vm->function_end = string_copyvalueof("functionend"); // TODO: This needs to be changed

	vm->print_function = string_copyvalueof("print");
	vm->read_function = string_copyvalueof("read");
	vm->write_function = string_copyvalueof("write");
	vm->system_function = string_copyvalueof("system"); // can call windows/linux/etc commands with this

	// Lists
	vm->global_variables = list_init();
	vm->function_list = list_init();
	vm->currentFunction = 0;

	return vm;
}

void vm_free(vm_t *vm) {
	string_free(vm->var_declare);
	string_free(vm->var_add);

	string_free(vm->goto_line);
	string_free(vm->goto_function);

	string_free(vm->function_declare);

	string_free(vm->print_function);
	string_free(vm->read_function);
	string_free(vm->write_function);
	string_free(vm->system_function);

	// Freeing Lists
	list_complete_free(&te_free, vm->global_variables);
	list_complete_free(&function_free, vm->function_list);

	free(vm);
}

void interpreter_ignition(FILE *stream, vm_t *virt) {
	interpreter_preprocessfile(stream, virt);
}

void interpreter_preprocessfile(FILE *stream, vm_t *vm) {
	string_t *line = string_init();
	int readStatus;
	while ((readStatus = readLine(line, stream)) != EOF) {
		parsed_instruction_t *instr = parse(vm->set_delimiter,
				vm->arg_delimiter, line);
		// If it is a function then store the args in a function_t struct
		// function hello -> hh, j
		if (string_startswith_s(instr->name, vm->function_declare)) {
			list_add(function_init(instr->args), vm->function_list);
			vm->currentFunction++;
			continue;
		}
		list_add(instr,
				((function_t*) vm->function_list[vm->currentFunction])->parsed_instructions);

		// If name is a function end, then the current function is below one
		if (string_equals_s(instr->name, vm->function_end))
			vm->currentFunction--; // this is almost like a higher level stack

		string_reset(line); // more efficient than freeing the string every time :)
	}
	// Free one string
	string_free(line);
}

void interpreter_execute(int lineNum, function_t *funct, vm_t *vm) {
	if (vm->currentFunction <= 0)
		return;

	for (int i = 0; i < funct->parsed_instructions->data_length; i++) {
		// How to handle functions
//		if (string_startswith_s(funct->parsed_instructions[lineNum], search) {
//
//		}

		// Core functions of the interpreter
		if (string_equals(funct->parsed_instructions[lineNum],
				vm->print_function))
			interpreter_print(funct->parsed_instructions[lineNum]->data);
		else if (string_equals(funct->parsed_instructions[lineNum],
				vm->print_function))
			interpreter_print();
	}
}

void interpreter_print(string_t *args) {

}

static int readLine(string_t *str, FILE *stream) {
	char letter;
	while ((letter = fgetc(stream)) != EOF) {
		if (letter == '\n')
			break;
		else
			string_appendchar(str, letter);
	}
	return letter;
}

parsed_instruction_t* parse(char set_delimiter, char arg_delimiter,
		string_t *line) {
	parsed_instruction_t *instr = malloc(sizeof(parsed_instruction_t));

	// New Syntax:
	// print -> "hello"
	// i -> 0
	string_t **two_pairs = string_split(set_delimiter, line);

	// If there is no "->" present in the system
	if (two_pairs == NULL)
		instr->name = parse_filteronlywords(line);

	instr->name = two_pairs[0];
	instr->args = parse_split_offquotes(arg_delimiter, two_pairs[1]);

	return instr;
}

static string_t* parse_filteronlywords(string_t *str) {
	string_t *newStr = string_init();
	for (int i = 0; i < str->text_length; i++)
		if (isalnum(str->text[i]))
			string_appendchar(newStr, str->text[i]);
	return newStr;
}

static list_t* parse_split_offquotes(char delimiter, string_t *line) {
	list_t *list = list_init();
	string_t *currentString = string_init();

	bool isString = false;
	for (int i = 0; i < line->text_length; i++) {
		if (line[i] == '"' || line[i] == '\'') {
			isString = !isString;
		} else if (!isString && line[i] == delimiter) {
			list_add(string_copyvalueof_s(currentString), list);
			string_reset(currentString);
		} else if (isString || line[i] != '\t') {
			string_append(currentString, line[i]);
		}

	}
	list_add(currentString);

	return list;
}

function_t* function_init(list_t *args) {
	function_t *funct = malloc(sizeof(function_t));

	funct->args = args;
	funct->parsed_instructions = list_init();
	funct->local_variables = list_init();
	return funct;
}

void function_free(void *funct) {
	list_complete_free(&te_free, ((function_t*) funct)->local_variables);
	list_complete_free(&parsed_instruction_free,
			((function_t*) funct)->parsed_instructions);
}

void parsed_instruction_free(void *instruction) {
	string_free(((parsed_instruction_t*) instruction)->name);
	list_complete_free(&string_free,
			((parsed_instruction_t*) instruction)->args);
}
