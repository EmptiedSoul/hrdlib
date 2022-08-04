#include <stdio.h>

#include "../libhrd.h"

void FILE_autofree(FILE** fp)
{
	fclose(*fp);
}
