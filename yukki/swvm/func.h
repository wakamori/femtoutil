#ifndef FUNC_H
#define FUNC_H

#define GLOBAL_VAR_MAX 64

struct cons_t;
struct Code;

typedef struct Func{
	char *name;
	struct cons_t *arg;
	int arg_count;
	struct Code *code;
	int code_index;
	struct Func *next;
}Func;

Func *add_function(char *name, struct cons_t *arg, struct Code *code, int code_index);
Func *get_function(char *name);
int is_argument(Func *f, char *name);


typedef struct Var{
	char *name;
	int value;
}Var;

extern Var global_var[GLOBAL_VAR_MAX];

int add_var(char *name);
int get_var(char *name);

#endif


