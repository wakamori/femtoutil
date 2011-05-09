#include <assert.h>

#define ml_print(...)\
	fprintf(stderr, __VA_ARGS__)

#ifdef MODE_DEBUG
#define DBG(...)\
	do {\
	ml_print("[%s:%d]\n", __FILE__, __LINE__);\
	ml_print(__VA_ARGS__);\
	ml_print("\n");\
	} while (0)
#else
#define DBG(...)
#endif
