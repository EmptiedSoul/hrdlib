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

extern char* _hrd_cache_dir; /* cache/cache.c */

size_t hrd_uncache_get_size(char* filename){
	if (_hrd_cache_dir == NULL){
		errno = ENOENT;
		return -1;
	} 
	if (filename == NULL){
		errno = EINVAL;
		return -1;
	}
	FILE* file = fopen(filename, "r");
	
	if (!file) return -1;

	(void)fseek(file, 0L, SEEK_END);
	size_t size = ftell(file);
	(void)rewind(file);
	fclose(file);
	return size; 
}

int hrd_uncache_get_data(char* filename, void* buffer, size_t size){
	if (_hrd_cache_dir == NULL){
		errno = ENOENT;
		return -1;
	} 
	if (filename == NULL){
		errno = EINVAL;
		return -1;
	}
	int file = open(filename, O_RDONLY);
	
	if (file == -1) return -1;
	
	void* addr = mmap(NULL, size, PROT_READ, MAP_PRIVATE, file, 0);
	(void)memcpy(buffer, addr, size);
	(void)munmap(addr, size);
	(void)close(file);
	return 0;
}
