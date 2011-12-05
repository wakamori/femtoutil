#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main (int ac, char **av)
{
  fprintf(stderr, "-----start program----\n");
  char *ptr = (char*)malloc(1024);
  memcpy(ptr, "hello world", sizeof("hello world"));
  int i = 0;
  for (i = 0; i < 1024; i++) {
	fprintf(stderr, "'%c'", (char)ptr[i]);
  }
  fprintf(stderr, "getted pointer : %p\n", ptr);
  fprintf(stderr, "------end program-----\n");
  return 0;
}
