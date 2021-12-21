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

int hrd_cfg_put_string_at(char* filename, char* key, char* value){
	FILE* stream = fopen(filename, "w");
	if (!stream)
		return -1;
	int res = hrd_cfg_put_string(stream, key, value);
	fclose(stream);
	return res;
}

int hrd_cfg_put_strings_at(char* filename, hrd_string_pair keys[]){
	FILE* stream = fopen(filename, "w");
	if (!stream)
		return -1;
	int res = hrd_cfg_put_strings(stream, keys);
	fclose(stream);
	return res;
}

int hrd_cfg_put_string(FILE* stream, char* key, char* value){
	hrd_string_pair* keys = alloca(sizeof(hrd_string_pair) * 2);
	hrd_string_pair  key_arg = {key, value};
	hrd_string_pair  terminator = {0,0};
	keys[0] = key_arg;
	keys[1] = terminator;
	return hrd_cfg_put_strings(stream, keys);
}

int hrd_cfg_put_strings(FILE* stream, hrd_string_pair keys[]){
	if (!stream) {
		errno = EINVAL;
		return -1;
	}
	int retval = 0;
	hrd_string_pair_array_foreach (i, keys) {
		retval = fprintf(stream, "%s=%s\n", keys[i].key, keys[i].value);
		if (retval < 0) break;
	}
	return retval;
}
