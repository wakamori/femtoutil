#include "molisp.h"

int main(int argc, const char *argv[])
{
	ml_print("Molisp version %d.%d\n", ML_VERSION_MAJOR, ML_VERSION_MINOR);
	if (argc > 1) {
		molisp_readfile(argv[1]);
	} else {
		molisp_interactive();
	}
	return 0;
}
