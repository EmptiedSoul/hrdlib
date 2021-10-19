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
	FILE* config = fopen(filename, "r");
	if (!config) return NULL;
	char* result = hrd_cfg_get_string(config, key);
	fclose(config);
	return result;
}

char* hrd_cfg_get_string(FILE* config, char* key){
	char* cfg_key = NULL;
	char* value = NULL;
	while (!feof(config)){
		int readsyms = fscanf(config, "%m[^=]=%m[^\n]\n", &cfg_key, &value);	
		switch (readsyms){
			case EOF:
				perror("hrd: cfg: get_string: fscanf");
				rewind(config);
				return NULL;
				break;
			case 0:
			case 1:
				fputs("hrd: Invalid config format\n", stderr);
				if (cfg_key != NULL) {
					char first = cfg_key[0];
					free(cfg_key);
					if (first == '#') break;
				}
				if (value != NULL)	free(value);
				errno = EINVAL;
				rewind(config);
				return NULL;
				break;
			case 2: 
				if (strcmp(cfg_key, key) == 0){
					free(cfg_key);
					rewind(config);
					return value;
				}else{
					free(cfg_key);
					free(value);
					value = NULL;
					cfg_key = NULL;
				}
				break;
		}
	}
	rewind(config);
	errno = ENOKEY;
	return NULL;
}
