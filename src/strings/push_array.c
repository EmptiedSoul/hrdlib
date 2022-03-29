#include "../libhrd.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void hrd_string_array_push(char*** array, char* string) {
	int i = 0;
	while ((*array)[i] != NULL) i++;
	*array = realloc(*array, sizeof(void*) * (i + 2));
	(*array)[i]   = strdup(string);
	(*array)[i+1] = NULL;
}
