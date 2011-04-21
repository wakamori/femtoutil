#include <stdlib.h>
#include "lisp.h"

static g_mallocedSize = 0;
//static var_t *free_list = NULL;

#define A(expr) assert(expr)

//#define LOW_OBJECT_SIZE (sizeof(var_t))
//#define LOW_OBJECT_NUM (1024)

/*
void low_allocator() 
{
  void *ptr = NULL;
  int i;

  ptr = low_malloc(LOW_OBJECT_SIZE * LOW_OBJECT_NUM);
  free_list = (var_t *)ptr;

  for(i=0; i<LOW_OBJECT_NUM; i++){
		free_list[i].next = &free_list[i+1];
  }
  free_list[LOW_OBJECT_NUM-1].next = NULL;
}

void low_freeVar(var_t *v)
{
	v->next = free_list;
	free_list = v;
}*/

void *low_malloc(size_t size)
{
  A(size >= 0);
  void *ret = malloc(size);
  if (ret == NULL){
	printf("malloc error");
  	exit(0);
  }
  g_mallocedSize += size;
  return ret;
}

void *low_free(void *ptr, size_t size)
{
  A(ptr != NULL);
  A(size >= 0);
  g_mallocedSize -= size;
  free(ptr);
}

