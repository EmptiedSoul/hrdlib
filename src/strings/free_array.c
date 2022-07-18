#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

#include "../libhrd.h"

void _hrd_autofree(void* memory)
{
	void** mem = (void**)memory;
	free(*mem);
}

void hrd_string_array_free(char** array)
{
	for (int i = 0; array[i]; i++) {
		free(array[i]);
	}
	free((unsigned long*)array - 1);
}

void hrd_string_array_autofree(char*** array)
{
	hrd_string_array_free(*array);
}

void hrd_string_pair_array_free(hrd_string_pair* array)
{
	hrd_string_pair_array_foreach(i, array)
	{
		free(array[i].key);
		if (array[i].value)
			free(array[i].value);
	}
	free(array);
}

void hrd_string_pair_array_free_stackkeys(hrd_string_pair* array)
{
	hrd_string_pair_array_foreach(i, array)
	{
		if (array[i].value)
			free(array[i].value);
	}
	free(array);
}

void hrd_string_pair_array_free_keysonly(hrd_string_pair* array)
{
	hrd_string_pair_array_foreach(i, array)
	{
		if (array[i].value)
			free(array[i].value);
	}
}
