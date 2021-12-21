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

char* hrd_cfg_get_string_at(char* filename, char* key){
	FILE* stream = fopen(filename, "r");
	if (!stream)
		return NULL;
	char* res = hrd_cfg_get_string(stream, key);
	fclose(stream);
	return res;
}

int hrd_cfg_get_strings_at(char* filename, hrd_string_pair* keys[]){
	FILE* stream = fopen(filename, "r");
	if (!stream)
		return -1;
	int res = hrd_cfg_get_strings(stream, keys);
	fclose(stream);
	return res;
}

char* hrd_cfg_get_string(FILE* stream, char* key){
	hrd_string_pair* keys = alloca(sizeof(hrd_string_pair) * 2);
	hrd_string_pair  key_arg = {key, 0};
	hrd_string_pair  terminator = {0,0};
	keys[0] = key_arg;
	keys[1] = terminator;
	int retval = hrd_cfg_get_strings(stream, &keys);
	if (retval < 0)
		return NULL;
	return keys[0].value;
}

int hrd_cfg_get_strings(FILE* stream, hrd_string_pair* keys[]){
	char*	line		= NULL;
	size_t	buflen		= 0;
	ssize_t	nread		= 0;
	int 	values_found	= 0;
	bool	read_failed 	= true;

	while ((nread = getline(&line, &buflen, stream)) != -1) {
		read_failed = false;
		if (*line == '#')
			continue;
		char* equal_sign = strchr(line, '=');
		if (!equal_sign) {
			errno = EINVAL;
			free(line);
			return -1;
		}
		char* value = equal_sign + 1;
		char* value_newline = strrchr(value, '\n');
		*value_newline = '\0';
		*equal_sign = '\0';
		char* key = line;
		hrd_string_pair_array_foreach (i, *keys) {
			if (strcmp((*keys)[i].key, key) == 0) {
				(*keys)[i].value = strdup(value);
				values_found++;
			}
		}
	}
	if (read_failed) {
		errno = EBADF;
		free(line);
		return -1;
	}

	free(line);
	return values_found;
}
