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
	int i = 0;

	token = strtok(string, delimiters);

	while (token != NULL) {
		tokens_array =
			(char**)realloc(tokens_array, sizeof(char*) * (i + 1));
		tokens_array[i] = (char*)malloc(strlen(token) + 1);
		memset(tokens_array[i], 0, strlen(token) + 1);
		strcpy(tokens_array[i], token);
		token = strtok(NULL, delimiters);
		i++;
	}

	tokens_array = (char**)realloc(tokens_array, sizeof(char*) * (i + 1));
	tokens_array[i] = NULL;

	return tokens_array;
}
