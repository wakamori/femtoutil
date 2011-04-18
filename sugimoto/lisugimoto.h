#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define TYPE_INT  0
#define TYPE_CHAR 1

typedef struct cons_t {
	int type;
	union {
		struct cons_t *car;
		char value;
		int ivalue;
		char *func;
	};
	struct cons_t *cdr;
} cons_t;

cons_t *sgmt_read(char *line);
cons_t *sgmt_eval(cons_t *root);
void sgmt_print(cons_t *root);
