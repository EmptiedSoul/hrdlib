#include "../libhrd.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void hrd_string_array_push(char*** array, char* string)
{
	unsigned long i = hrd_string_array_get_length(*array);
	void* mem = realloc((unsigned long*)*array - 1,
			    sizeof(char*) * (i + 2) + sizeof(unsigned long));
	*array = mem + sizeof(unsigned long);
	(*array)[i] = strdup(string);
	(*array)[i + 1] = NULL;
	unsigned long* num = (unsigned long*)*array - 1;
	*num = i + 1;
}
