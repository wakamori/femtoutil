#ifndef LIST_H
#define LIST_H

typedef struct var_t{
	const char *name;
	int value;
	struct var_t *next;
}var_t;

typedef struct func_t{
	const char *name;
	cons_t *args;
	cons_t *car;
	struct func_t *next;
}func_t;

var_t *set_var_value(var_t *top, const char *name, int value);
var_t *get_var_value(var_t *top, const char *name);

func_t *set_func(func_t *top, const char *name, cons_t *args, cons_t *car);
func_t *get_func(func_t *top, const char *name);

#endif

