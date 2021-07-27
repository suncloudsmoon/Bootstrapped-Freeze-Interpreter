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
 * listobj.h
 *
 *  Created on: Jul 25, 2021
 *      Author: suncloudsmoon
 */

#ifndef LISTOBJ_H_
#define LISTOBJ_H_

#include <stdio.h>

#define LIST_MANAGER_ALLOC_SIZE 10

typedef struct {
	void **data;

	int data_length;
	int data_allocated_length;
} list_t;

list_t* list_init();

void list_add(void *item, list_t *list);

void list_remove(int index, list_t *list);
/**
 * Completely frees the individual item to be removed by calling the respective free method
 * of the data type (like freeing structs).
 */
void list_complete_remove(void (*indivfree) (void *), int index, list_t *list);

void list_clear(list_t *list);

/**
 * Compares the data according to how the data must be compared with the equalsComparator function
 */
bool list_equals(void *destComp, int index, bool (*equalsComparator) (void*, void*), list_t *list);
bool list_contains(void *destComp, bool *(equalsComparator) (void *, void *), list_t *list);

void list_serialize(void (*indiv) (void *, FILE *), FILE *stream, list_t *list);
list_t* list_deserialize(void* (*indivreverse) (FILE *));

void list_free(list_t *list);
/**
 * Frees the individual data (like structs) inside the void pointer.
 */
void list_complete_free(void (*indivfree) (void *), list_t *list);

#endif /* LISTOBJ_H_ */
