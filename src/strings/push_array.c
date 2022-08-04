#include "../libhrd.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void hrd_string_array_push(char*** array, char* string)
{
	void* mem = NULL;
	unsigned long i = hrd_string_array_get_length(*array);
	void* rlc_tmp =
		realloc((unsigned long*)*array - 1,
			sizeof(char*) * (i + 2) + sizeof(unsigned long));
	if (rlc_tmp != NULL)
		mem = rlc_tmp;
	else
		goto oom;
	*array = mem + sizeof(unsigned long);
	(*array)[i] = strdup(string);
	(*array)[i + 1] = NULL;
	unsigned long* num = (unsigned long*)*array - 1;
	*num = i + 1;
oom:
	return;
}
