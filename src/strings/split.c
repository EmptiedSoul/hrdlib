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

char** __attribute__((malloc)) hrd_string_split(char* string, char* delimiters)
{
	char** tokens_array = NULL;
	char* token = NULL;
	unsigned long int i = 0;
	size_t long_size = sizeof(unsigned long);
	char* tmp = NULL;

	void* mem = malloc(long_size);
	if (!mem)
		return NULL;
	void* rlc_tmp = NULL;

	token = strtok_r(string, delimiters, &tmp);

	while (token != NULL) {
		rlc_tmp = realloc(mem, sizeof(char*) * (i + 1) + long_size);
		if (rlc_tmp != NULL)
			mem = rlc_tmp;
		else
			goto oom;
		tokens_array = mem + long_size;
		tokens_array[i] = (char*)malloc(strlen(token) + 1);
		if (!tokens_array[i])
			goto oom;
		memset(tokens_array[i], 0, strlen(token) + 1);
		strcpy(tokens_array[i], token);
		token = strtok_r(NULL, delimiters, &tmp);
		i++;
	}

	rlc_tmp = realloc(mem, sizeof(char*) * (i + 1) + long_size);
	if (rlc_tmp != NULL)
		mem = rlc_tmp;
	else
		goto oom;
	tokens_array = mem + long_size;
	tokens_array[i] = NULL;

	unsigned long* num = mem;
	*num = i;

	return tokens_array;
oom:
	for (unsigned long long j = 0; j < i; j++) {
		free(tokens_array[j]);
	}
	free(mem);
	return NULL;
}
