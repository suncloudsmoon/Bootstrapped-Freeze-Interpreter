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
 * - Use malloc when you will overwrite the data anyway
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

	return str;
}

static string_t* custom_string_init(int allocationSize) {
	string_t *str = malloc(sizeof(string_t));

	str->text = malloc(allocationSize * sizeof(char));
	str->text[0] = '\0';

	str->text_length = 0;
	str->text_allocated_length = allocationSize;

	return str;
}

string_t* string_copyvalueof(char *src) {
	int srcLength = strlen(src);

	string_t *newStr = custom_string_init(srcLength + STRING_ALLOCATION_SIZE);
	strncpy(newStr->text, src, srcLength);
	newStr->text_length = srcLength;

	return newStr;
}

string_t* string_copyvalueof_s(string_t *src) {
	// QUESTION: does memcpy() even work?
	string_t *dest = custom_string_init(src->text_allocated_length);
	strncpy(dest->text, src->text, src->text_length);
	dest->text_length = src->text_length;

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

void string_appendchar(string_t *dest, char letter) {
	string_meminspection(1, dest);

//	char tempLetter[2];
//	tempLetter[0] = letter;
//	tempLetter[1] = '\0';
//	strncat(dest->text, tempLetter, 1);
	dest->text[dest->text_length] = letter;
	dest->text[dest->text_length + 1] = '\0';
}

string_t** string_split(char delimiter, string_t *src) {
	// Safety
	// If the string is length 2 or less, then it is not possible to split the string
	if (src->text_length <= 2)
		throw_exception(INDEX_OUT_OF_BOUNDS_EXCEPTION, -1,
				"Unable to substring a string with %d length!",
				src->text_length);

	string_t **strList = malloc(2 * sizeof(string_t));
	strList[0] = custom_string_init(src->text_length / 2);
	strList[1] = custom_string_init(src->text_length / 2);

	char delimiterText[2];
	delimiterText[0] = delimiter;
	delimiterText[1] = '\0';
	int splitIndex = strcspn(src->text, delimiterText);

	if (splitIndex == src->text) {
		return NULL;
	}

	for (int i = 0; i < splitIndex; i++) {
		string_appendchar(strList[0], src->text[i]);
	}
	for (int i = splitIndex + 1; i < src->text_length; i++) {
		string_appendchar(strList[1], src->text[i]);
	}

	return strList;
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
	if (dest->text_length != strlen(src)) {
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

bool string_startswith_s(string_t *src, string_t *search) {
	return strcspn(src->text, search->text) != src->text_length;
}

void string_tolowercase_s(string_t *dest) {
	for (int i = 0; i < dest->text_length; i++)
		dest->text[i] = tolower(dest->text[i]);
//	for (char *letter = dest->text; *letter; letter++)
//		*letter = tolower(*letter);
}

void string_reset(string_t *dest) {
	dest->text[0] = '\0';
	dest->text_length = 0;
}

void string_serialize(string_t *src, FILE *stream) {
	fwrite(&src->text_length, sizeof(int), 1, stream);
	fwrite(src->text, sizeof(char), src->text_length, stream);
}

string_t* string_deserialize(FILE *stream) {
	int textLength;
	fread(&textLength, sizeof(int), 1, stream);

	string_t *str = custom_string_init(textLength + STRING_ALLOCATION_SIZE);
	fread(str->text, sizeof(char), textLength, stream);
	str->text_length = textLength;

	return str;
}

void string_free(void *dest) {
	// Free string inside dest
	free(((string_t*) dest)->text);
	// Free the structure itself
	free(((string_t*) dest));
}

// Memory related functions
static void string_meminspection(int addNum, string_t *subject) {
	if (subject->text_length + addNum + 1 >= subject->text_allocated_length) {
		addNum += subject->text_length / 2;
		char *tempStr = (char*) realloc(subject->text, addNum * sizeof(char));

		// Safety
		if (tempStr == NULL)
			throw_exception(NULL_POINTER_EXCEPTION, -1,
					"Unable to reallocate memory for string!");

		subject->text = tempStr;
		subject->text_allocated_length += addNum;
	}
}
