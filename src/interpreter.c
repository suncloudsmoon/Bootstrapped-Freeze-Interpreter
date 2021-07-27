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
vm_t** vm_init(FILE *stream) {

}

vm_t* vm_free() {

}

void interpreter_ignition(vm_t *virt) {

}

void interpreter_preprocessfile(FILE *stream) {
	string_t *line = string_init();
	int readStatus;
	while ((readStatus = readLine(line, stream)) != EOF) {

		string_reset(line); // more efficient :)
	}
	// Free Resources
	string_free(line);
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

parsed_instruction_t* parse(string_t *delimiter, string_t *line) {
	parsed_instruction_t *instr = malloc(sizeof(parsed_instruction_t));
	string_t **two_pairs = string_split(delimiter, line);
	instr->name = p
	instr->args = parse_split_offquotes(delimiter, two_pairs[1]);
}

static list_t* parse_split_offquotes(string_t *delimiter, string_t *line) {
	list_t *list = list_init();
	string_t *currentString = string_init();

	bool isString = false;
	for (int i = 0; i < line->text_length; i++) {
		if (line[i] == '"' || line[i] == '\'') {
			isString = !isString;
		} else if (!isString && line[i] == ',') {
			list_add(string_copyvalueof_s(currentString), list);
			string_reset(currentString);
		}
		string_append(currentString, line[i]);
	}
	lista_add(currentString);

	return list;
}
