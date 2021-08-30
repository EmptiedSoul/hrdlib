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

void hrd_array_free(void** array){
	for (int i = 0; array[i]; i++){
		free(array[i]);
	}
	free(array);
}
