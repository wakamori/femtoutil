#include <stdlib.h>
#include "lisp.h"
#include "memory.h"

void *low_malloc(size_t size)
{
	return malloc(size);
}

void low_free(void *ptr, size_t size)
{
	free(ptr);
}

