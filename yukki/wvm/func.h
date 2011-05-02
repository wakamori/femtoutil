#ifndef FUNC_H
#define FUNC_H

struct cons_t;
struct Code;

typedef struct Func{
	char *name;
	struct cons_t *arg;
	struct Code *code;
	int code_index;
	struct Func *next;
}Func;

Func *add_function(char *name, struct cons_t *arg, struct Code *code, int code_index);
Func *get_function(char *name);
int is_argument(Func *f, char *name);

#endif


