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

char** hrd_string_split(char* string, char* delimiters)
{
	char** tokens_array = NULL;
	char* token = NULL;
	unsigned long int i = 0;
	size_t long_size = sizeof(unsigned long);

	token = strtok(string, delimiters);

	void* mem = malloc(long_size);

	while (token != NULL) {
		mem = realloc(mem, sizeof(char*) * (i + 1) + long_size);
		tokens_array = mem + long_size;
		tokens_array[i] = (char*)malloc(strlen(token) + 1);
		memset(tokens_array[i], 0, strlen(token) + 1);
		strcpy(tokens_array[i], token);
		token = strtok(NULL, delimiters);
		i++;
	}

	mem = realloc(mem, sizeof(char*) * (i + 1) + long_size);
	tokens_array = mem + long_size;
	tokens_array[i] = NULL;

	unsigned long* num = mem;
	*num = i;

	return tokens_array;
}
