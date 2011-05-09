#include <stdlib.h>
#include "lisp.h"

#define A(expr) assert(expr)

void *low_malloc(size_t size)
{
  A(size >= 0);
  void *ret = malloc(size);
  if (ret == NULL){
		printf("malloc error");
  	exit(0);
  }
  return ret;
}

void low_free(void *ptr)
{
	free(ptr);
}

