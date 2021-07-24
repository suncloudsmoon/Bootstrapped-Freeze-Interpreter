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
 * stringobj.c
 *
 *  Created on: Jul 23, 2021
 *      Author: suncloudsmoon
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "../include/stringobj.h"
#include "../include/throwable.h"

/*
 * Coding philosophy here:
 * - Use calloc when necessary, not everywhere
 */

// Static prototypes
static string_t* custom_string_init(int allocationSize);
static void string_meminspection(int addNum, string_t *subject);

string_t* string_init() {
	string_t *str = malloc(sizeof(string_t));

	str->text = malloc(STRING_ALLOCATION_SIZE * sizeof(char));
	str->text[0] = '\0';

	str->text_length = 0;
	str->text_allocated_length = STRING_ALLOCATION_SIZE;
}

static string_t* custom_string_init(int allocationSize) {
	string_t *str = malloc(sizeof(string_t));

	str->text = malloc(allocationSize * sizeof(char));
	str->text[0] = '\0';

	str->text_length = 0;
	str->text_allocated_length = allocationSize;
}

string_t* string_copyvalueof(char *src) {
	int srcLength = strlen(src);
	string_t *newStr = custom_string_init(srcLength + STRING_ALLOCATION_SIZE);
	strncpy(newStr->text, src, srcLength);

	return newStr;
}

string_t* string_copyvalueof_s(string_t *src) {
	string_t *dest;
	memcpy(dest, src, sizeof(src)); // QUESTION: does memcpy() even work?

	return dest;
}

void string_append(string_t *dest, char *src) {
	int srcLength = strlen(src);
	string_meminspection(srcLength, dest);

	strncat(dest->text, src, srcLength);
	dest->text_length += srcLength;
}

void string_append_s(string_t *dest, string_t *src) {
	string_meminspection(src->text_length, dest);
	strncat(dest->text, src->text, src->text_length);
	dest->text_length += src->text_length;
}

bool string_equals(string_t *dest, char *src) {
	int srcLength = strlen(src);
	if (dest->text_length != srcLength)
		return false;
	else
		return strncmp(dest->text, src, srcLength) == 0;
}

bool string_equals_s(string_t *dest, string_t *src) {
	if (dest->text_length != src->text_length)
		return false;
	else
		return strncmp(dest->text, src, src->text_length) == 0;
}

bool string_equalsignorecase(string_t *dest, char *src) {
	int srcLength = strlen(src);
	if (dest->text_length != srcLength) {
		return false;
	} else {
		for (int i = 0; i < dest->text_length; i++)
			if (tolower(dest->text[i]) != tolower(src[i]))
				return false;
		return true;
	}
}

bool string_equalsignorecase_s(string_t *dest, string_t *src) {
	if (dest->text_length != src->text_length) {
		return false;
	} else {
		for (int i = 0; i < dest->text_length; i++)
			if (tolower(dest->text[i]) != tolower(src->text[i]))
				return false;
		return true;
	}
}

void string_tolowercase_s(string_t *dest) {
	for (int i = 0; i < dest->text_length; i++)
		dest->text[i] = tolower(dest->text[i]);
}

void string_reset(string_t *dest) {
	dest->text[0] = '\0';
	dest->text_length = 0;
}

void string_free(string_t *dest) {
	// Free string inside dest
	free(dest->text);
	// Free the structure itself
	free(dest);
}

// Memory related functions
static void string_meminspection(int addNum, string_t *subject) {
	if (subject->text_length + addNum + 1 >= subject->text_allocated_length) {
		int strlen_diff = (subject->text_length + addNum + 1)
				- subject->text_allocated_length;
		char *tempStr = (char*) realloc(subject->text,
				(strlen_diff + STRING_ALLOCATION_SIZE) * sizeof(char));
		// Safety
		if (tempStr != NULL) {
			subject->text = tempStr;
		} else {
			throw_exception(NULL_POINTER_EXCEPTION, -1,
					"Unable to reallocate memory for string!");
		}
		subject->text_allocated_length += strlen_diff + STRING_ALLOCATION_SIZE;
	}
}
