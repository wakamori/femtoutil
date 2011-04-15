#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#define NIL             ((cell)0)
#define CAR(v)          ((cons_t *)v)->car_v
#define CDR(v)          ((cons_t *)v)->cdr_v

typedef struct cons_t {
	int type;
	union {
		struct cons_t *car;
		char *value;
		int ivalue;
	}v;
	struct cons_t *cdr;
} cons_t;

cons_t *sgmt_read(char *line);
cons_t *sgmt_eval(cons_t *root);
void sgmt_print(cons_t *root);
