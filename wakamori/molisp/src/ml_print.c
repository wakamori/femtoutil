#include "molisp.h"

void ml_error(char message[])
{
	fprintf(stderr, "[error] %s\n", message);
}
