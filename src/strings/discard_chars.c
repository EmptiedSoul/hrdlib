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

size_t hrd_string_discard_chars(char* string, char to_discard)
{
	hrd_autofree char* buf = NULL;

	size_t strsize = strlen(string);
	buf = (char*)malloc(strsize);

	int discarded_num = 0;

	for (size_t i = 0; i<strsize; i++){
		if (string[i] != to_discard)
			buf[i - discarded_num] = string[i];
		else
			discarded_num++;
	}
	
	if (!discarded_num) return strsize;

	buf[strsize - discarded_num] = '\0';
	
	size_t written = strlen(buf);
	strcpy(string, buf);

	return written;
}
