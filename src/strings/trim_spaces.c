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
#include <ctype.h>

#include "../libhrd.h"

void hrd_trim_spaces(char* string)
{
	char* p = string;
	int l = strlen(p);

	while (isspace(p[l - 1])) {
		p[--l] = 0;
	}
	while (*p && isspace(*p)) {
		++p;
		--l;
	}

	memmove(string, p, l + 1);
}
