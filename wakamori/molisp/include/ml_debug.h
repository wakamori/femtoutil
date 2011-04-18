#include <assert.h>
#define DBG1(str) \
	fprintf(stderr, "[%s:%d]\n" str "\n", __FILE__, __LINE__)
#define DBG2(fmt, ...) \
	fprintf(stderr, "[%s:%d]\n" fmt "\n", __FILE__, __LINE__, __VA_ARGS__)
