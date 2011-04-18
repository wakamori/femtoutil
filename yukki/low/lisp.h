#ifndef LISP_H
#define LISP_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TYPE_INT 1
#define TYPE_CAR 2
#define TYPE_OPERATE 3
#define TYPE_NIL 4
#define TYPE_T 5
#define TYPE_IF 6
#define TYPE_SETQ 7
#define TYPE_DEFUN 8
#define TYPE_STR 9

typedef union consvalue_t{
	struct cons_t *car;
	char *str;
	int i;
}consvalue_t;

typedef struct cons_t{
	int type;
	consvalue_t v;
	struct cons_t *cdr;
}cons_t;

#define TOKEN_NULL 0
#define TOKEN_INT 1
#define TOKEN_STR 2

typedef struct Token{
	int type;
	union{
		int num;
		char str[64];
	};
	char *input;
}Token;

struct var_t;

cons_t *create_list(Token *token);
cons_t eval(cons_t *list, struct var_t *local_vars);
void free_cons(cons_t *);

// allocater
struct var_t *low_newVar();
void low_freeVar(struct var_t *);

void *low_malloc(size_t size);


#endif

