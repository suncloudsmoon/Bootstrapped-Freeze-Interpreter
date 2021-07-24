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
 * throwable.c
 *
 *  Created on: Jul 23, 2021
 *      Author: suncloudsmoon
 */

#ifndef THROWABLE_C_
#define THROWABLE_C_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "../include/throwable.h"

void throw_exception(exception e, int lineNum, char *message) {
	char cMessage[50];
	if (lineNum == -1)
		sprintf(cMessage, "Internal Error");
	else
		sprintf(cMessage, "Line #%d", lineNum);

	// Goes through the different types of error and prints out the appropriate message
	switch (e) {
	case ERRNO_EXCEPTION:
		perror(cMessage);
		break;
	case NULL_POINTER_EXCEPTION:
		fprintf(stderr, "%s: %s\n", cMessage, message);
		break;
	default:
		break;
	}
}

#endif /* THROWABLE_C_ */
