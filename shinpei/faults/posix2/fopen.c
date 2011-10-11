#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <dlfcn.h>


FILE *(*fopen_org) (const char *path, const char *mode) = NULL;

__attribute__((constructor))
void 
_save_original_functions()
{
  fopen_org = (FILE*(*)(const char *, const char *)) dlsym(RTLD_NEXT, "fopen");
}

extern int errno;
FILE *fopen(const char *path, const char *mode) {
  // check its usage
  int fd = open("fault.txt", 0);
  if (fd != -1) {
	char buf[4];
	int ret = read(fd, buf, 2);
	int err = atoi(buf);
	fprintf(stderr, "fault injected:%d!\n", err);
	errno = err;
	return NULL;
  } else {
	return fopen_org(path, mode);
  }
}
