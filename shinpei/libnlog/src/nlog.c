/*****************************
 *
 * libnlog 
 * shinpei nakata(c)2011
 *
 *****************************/
#include <nlog.h>

int get_nlogtime(void)
{
  return 1;
}

void nlog_dump(const char *tname, nlog_t nl[])
{
  nlog2_t *cur = (nlog2_t*)nl;
  char buf[256] = {0};
  buf[0] = '{';
  const char *fmt;
  char *bufptr = buf + 1;
  while (cur->type != 0) {
	switch (cur->type) {
	case LOGT_i:
	  fmt = "%s:%d,";
	  break;
	case LOGT_p:
	  fmt = "%s:%p,";
	  break;
	case LOGT_s:
	  fmt = "%s:%s,";
	  break;
	default:
	  fmt = "%s:%s,";
	  break;
	}
	sprintf(bufptr,  fmt, cur->key, cur->value);
	bufptr += strlen(bufptr);
	cur++;
  }
  bufptr[-1] = '}';
  
  fprintf(stderr, "%s:%s\n", tname, buf);
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
