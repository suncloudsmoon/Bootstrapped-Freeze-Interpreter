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
 * interpreter.h
 *
 *  Created on: Jul 23, 2021
 *      Author: suncloudsmoon
 */

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include "../include/stringobj.h"
#include "../include/listobj.h"
#include "../deps/tinyexpr/tinyexpr.h"

typedef struct {
	string_t *name;
	list_t *args;
} parsed_instruction_t;

// Everything is a function in my language :)

typedef struct {
	list_t *args;
	list_t *parsed_instructions; // List of parsed_instruction_t
	list_t *local_variables; // LOOK: hopefully this works because I was supposed to use te_variable array
} function_t;

typedef struct {
	// Core internal features of the interpreter
	char set_delimiter;
	char arg_delimiter;

	// Function identifiers
	string_t *function_declare, *function_end;

	string_t *var_declare, *var_add;
	string_t *goto_line, *goto_function;

	string_t *print_function, *read_function, *write_function, *system_function;

	// Lists
	list_t *global_variables; // list of te_variable structs
	list_t *function_list; // list of function_t structs
	int currentFunction;

} vm_t; // Short and simple name

/**
 * Initializes the interpreter's virtual machine, where every variable and other stuff is stored that is
 * going to be used throughout the process of the interpreter
 */
vm_t* vm_init();
/**
 * Frees whatever the interpreter has been storing in the vm_t structure.
 */
void vm_free(vm_t *vm);

/**
 * Starts the interpreter provided a file containing the interpreter code
 * Inspiration from the V8's ignition
 */
void interpreter_ignition(FILE *stream, vm_t *virt);
void interpreter_preprocessfile(FILE *stream, vm_t *vm);
parsed_instruction_t* parse(char whitespace_delimiter, char arg_delimiter,
		string_t *line);

function_t* function_init(list_t *args);
void function_free(void *funct);

void parsed_instruction_free(void *instruction);

#endif /* INTERPRETER_H_ */
