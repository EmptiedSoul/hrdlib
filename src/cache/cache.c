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

char* _hrd_cache_dir = NULL;

int hrd_cache_set_dir(char* dir){
	if (hrd_file_exist(dir, HRDFS_ISDIR)){
		_hrd_cache_dir = dir;
	} else {
		errno = ENOENT;
		return -1;
	}
	return 0;
}

int hrd_cache_data(char* filename, void* data, size_t size){
	if (_hrd_cache_dir == NULL){
		errno = ENOENT;
		return -1;
	} 
	if (filename == NULL){
		errno = EINVAL;
		return -1;
	}
	if (size <= 0){
		errno = EINVAL;
		return -1;
	}

	size_t strsize = strlen(_hrd_cache_dir) + strlen(filename) + 1;
	char* buf = malloc(strsize);
	memset(buf, '\0', strsize);

	(void)strcat(buf, _hrd_cache_dir);
	(void)strcat(buf, filename);
	
	int file = open(buf, O_RDWR | O_CREAT, 0600);
	(void)free(buf);
	
	void* addr = mmap(NULL, size, PROT_WRITE, MAP_SHARED, file, 0);

	(void)ftruncate(file, size);
	(void)memcpy(addr, data, size);

	(void)munmap(addr, size);
	(void)close(file);

	return 0;
}
