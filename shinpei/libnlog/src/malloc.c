/*****************************
 *
 * libnlog 
 * shinpei nakata(c)2011
 *
 *****************************/

#include <nlog.h>


static void install_malloc(void)
{
  malloc_orig = __malloc_hook;
  __malloc_hook = nlog_malloc;
}

static void uninstall_malloc(void)
{
  __malloc_hook = malloc_orig;
  
}

void *nlog_malloc (size_t size, const void *caller)
{
  uninstall_malloc();
  void *ptr = malloc(size);
  nlog_t nl[] = {LOG_i("size", size), LOG_p("caller", caller), LOG_END};
  nlog_dump("malloc", nl);
  install_malloc();
  return ptr;
}

__attribute__((constructor))
void
 __nlog_initialize(void)
{
  if (malloc_orig == NULL) {
	malloc_orig = __malloc_hook;
	__malloc_hook = nlog_malloc;
  }
}
