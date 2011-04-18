#include <stdlib.h>
#include "lisp.h"
#include "list.h"

static g_mallocedSize = 0;

#define A(expr) assert(expr)

#define LOW_OBJECT_SIZE (sizeof(var_t))
#define LOW_OBJECT_NUM (1024)

void low_allocator() 
{
  void *ptr = NULL;

  ptr = low_malloc(LOW_OBJECT_SIZE * LOW_OBJECT_NUM);
  

}

var_t *low_newVar()
{
  
}

void *low_malloc(size_t size)
{
  A(size >= 0);
  void *ret = malloc(size);
  if (){ } // error handler
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

