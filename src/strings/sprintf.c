/* Copyright (c) 2022, Arseniy "Emptied Soul" Lesin
 * License: GPLv3
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../libhrd.h"

char* hrd_sprintf(char* fmt, ...) {
	va_list ap;
	char* str;

	va_start(ap, fmt);
	if (vasprintf(&str, fmt, ap) == -1)
		str = NULL; 
	va_end(ap);

	return str;
}
