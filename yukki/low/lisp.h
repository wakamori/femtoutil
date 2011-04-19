#ifndef LISP_H
#define LISP_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum Types{
	TYPE_NIL,
	TYPE_T,
	TYPE_INT,
	TYPE_CAR,
	TYPE_OPERATE,
	TYPE_COMPARE,
	TYPE_IF,
	TYPE_SETQ,
	TYPE_DEFUN,
	TYPE_STR
};

typedef union consvalue_t{
	struct cons_t *car;
	char *str;
	int i;
	int (*func)(int x, int y);
}consvalue_t;

typedef struct cons_t{
	int type;
	consvalue_t v;
	struct cons_t *cdr;
}cons_t;

enum Tokens{
	TOKEN_INT,
	TOKEN_STR,
	TOKEN_OPERATE,
	TOKEN_COMPARE,
	TOKEN_BRACE_OPEN,
	TOKEN_BRACE_CLOSE
};

enum Operaters{
	OP_PLUS,
	OP_MINUS,
	OP_MULTI,
	OP_DIV,
	OP_MOD,

	OP_GT,
	OP_GE,
	OP_LT,
	OP_LE,
	OP_EQ
};

typedef struct Token{
	int type;
	union{
		int num;
		char str[64];
		int (*ope)(int x, int y);
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

