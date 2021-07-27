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
 * listobj.c
 *
 *  Created on: Jul 25, 2021
 *      Author: suncloudsmoon
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/listobj.h"
#include "../include/throwable.h"

// Static Prototypes
static list_t* custom_list_init(int mallocSize);
static void list_meminspector(int addNum, list_t *subject);

/*
 * Need to add:
 * - support for sorting like - int list_sort(void (*funct) (void *, void *), list_t *list)
 */

list_t* list_init() {
	list_t *list = malloc(sizeof(list_t));
	list->data = (void**) malloc(LIST_MANAGER_ALLOC_SIZE * sizeof(void*));

	list->data_length = 0;
	list->data_allocated_length = LIST_MANAGER_ALLOC_SIZE;
}

static list_t* custom_list_init(int mallocSize) {
	list_t *list = malloc(sizeof(list_t));
	list->data = (void**) malloc(mallocSize * sizeof(void*));

	list->data_length = 0;
	list->data_allocated_length = mallocSize;
}

void list_add(void *item, list_t *list) {
	list_meminspector(1, list);
	list->data[list->data_length] = item;
	list->data_length++;
}

void list_remove(int index, list_t *list) {
	// TODO: find a more efficient implementation of this
	for (int i = index; i < list->data_length - 1; i++)
		list->data[i] = list->data[i + 1];

	list->data_length--;
}

void list_complete_remove(void (*indivfree)(void*), int index, list_t *list) {
	(*indivfree)(list->data[index]); // frees it from the respective free method for the unknown type data
	list_remove(index, list);
}

void list_clear(list_t *list) {
	list->data_length = 0;
}

bool list_equals(void *destComp, int index,
bool (*equalsComparator)(void*, void*), list_t *list) {
	if (index < 0 || index >= list->data_length)
		throw_exception(INDEX_OUT_OF_BOUNDS_EXCEPTION, -1,
				"Tried to access a list in index %d that was out of bounds!",
				index);

	return (*equalsComparator)(destComp, list->data[index]);
}

bool list_contains(void *destComp, bool (*equalsComparator)(void*, void*),
		list_t *list) {
	for (int i = 0; i < list->data_length; i++)
		if ((*equalsComparator)(destComp, list->data[i]))
			return true;
	return false;
}

void list_serialize(void (*indiv)(void*, FILE*), FILE *stream, list_t *list) {
	fwrite(list->data_length, sizeof(int), 1, stream);
	for (int i = 0; i < list->data_length; i++)
		(*indiv)(list->data[i], stream);
}

list_t* list_deserialize(void* (*indivreverse)(FILE*), FILE *stream) {
	int arrayLength;
	fread(&arrayLength, sizeof(int), 1, stream);

	list_t *list = custom_list_init(arrayLength);
	for (int i = 0; i < arrayLength; i++)
		list_add((*indivreverse)(stream), list);

	return list;
}

void list_free(list_t *list) {
	free(list->data);
	free(list);
}

void list_complete_free(void (*indivfree)(void*), list_t *list) {
	for (int i = 0; i < list->data_length; i++)
		(*indivfree)(list->data[i]);
	list_free(list);
}

static void list_meminspector(int addNum, list_t *subject) {
	if (subject->data_length + addNum >= subject->data_allocated_length) {
		addNum += subject->data_length / 2;
		void **new_ptr = (void**) realloc(new_ptr, addNum * sizeof(void*));
		if (new_ptr == NULL)
			throw_exception(NULL_POINTER_EXCEPTION, -1,
					"Unable to allocate memory for list with length %d!",
					subject->data_length);

		subject->data = new_ptr;
		subject->data_allocated_length += addNum;
	}
}
