/*****************************
 *
 * libnlog 
 * shinpei nakata(c)2011
 *
 *****************************/
#include <nlog.h>

void nlog_dump(const char *tname, nlog_t nl[])
{
  nlog2_t *cur = nl;
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
