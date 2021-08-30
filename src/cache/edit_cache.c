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

void* hrd_cache_edit_data(char* filename, size_t size){
	if (_hrd_cache_dir == NULL){
		(void)fputs("hrd: Attempted to edit cached data with unsetted cache_dir\n", stderr);
		errno = ENOENT;
		return NULL;
	} 
	if (filename == NULL){
		(void)fputs("hrd: Attempted to edit cached data without filenamei\n", stderr);
		errno = EINVAL;
		return NULL;
	}
	if (!hrd_file_exist(filename, HRDFS_ISFILE | HRDFS_READBL | HRDFS_WRITBL)){
		(void)fputs("hrd: Attempted to edit cached data from directory/file without read-write permissions/missing file\n", stderr);
		errno = EACCES;
		return NULL;
	}
	int file = open(filename, O_RDWR);
	void* buffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
	(void)close(file);
	return buffer;
}

int hrd_cache_edit_end(void* buffer, size_t size){
	return munmap(buffer, size);	
}
