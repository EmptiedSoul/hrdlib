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

char* hrd_cfg_get_string(char* filename, char* key){
	if (!hrd_file_exist(filename, HRDFS_ISFILE|HRDFS_READBL )) {
		fputs("hrd: Attempted to read string from non-existent file\n", stderr);
		errno = ENOENT;
		return NULL;
	}

	char* cfg_key = NULL;
	char* value = NULL;

	FILE* config = fopen(filename, "r");
	while (!feof(config)){
		int readsyms = fscanf(config, "%m[^=]=%m[^\n]\n", &cfg_key, &value);	
		switch (readsyms){
			case EOF:
				perror("hrd: cfg: get_string: fscanf");
				fclose(config);
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
				fclose(config);
				errno = EINVAL;
				return NULL;
				break;
			case 2: 
				if (strcmp(cfg_key, key) == 0){
					free(cfg_key);
					fclose(config);
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
	fclose(config);
	errno = ENOKEY;
	return NULL;
}
