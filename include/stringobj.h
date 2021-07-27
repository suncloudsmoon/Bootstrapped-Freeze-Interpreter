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
 * stringobj.h
 *
 *  Created on: Jul 23, 2021
 *      Author: suncloudsmoon
 */

#ifndef STRINGOBJ_H_
#define STRINGOBJ_H_

#include <stdio.h>
#include <stdbool.h>

#define STRING_ALLOCATION_SIZE 5

/*
 * Philosophy:
 * - We need maximum interoperability between the C string lib and our custom string lib to get the
 * maximum performance, flexibility, security, and ease of use
 *
 * Note:
 * - the "_s" at the end of functions mean that you are comparing two strings defined in our library
 */

typedef struct {
	char *text;

	int text_length;
	int text_allocated_length;
} string_t;

string_t *string_init();

string_t *string_copyvalueof(char *text);
string_t *string_copyvalueof_s(string_t *src);

void string_append(string_t *dest, char *src);
void string_append_s(string_t *dest, string_t *src);

void string_appendchar(string_t *dest, char letter);

/**
 * Returns 2 strings that are split from the delimiters
 */
string_t** string_split(char *delimiter);

// Comparisons between strings
bool string_equals(string_t *dest, char *src);
bool string_equals_s(string_t *dest, string_t *src);

bool string_equalsignorecase(string_t *dest, char *src);
bool string_equalsignorecase_s(string_t *dest, string_t *src);

// Making modifications to the string
void string_tolowercase_s(string_t *dest);
void string_reset(string_t *dest);

void string_serialize(string_t *src, FILE *stream);
string_t* string_deserialize(FILE *stream);

void string_free(string_t *dest);

#endif /* STRINGOBJ_H_ */
