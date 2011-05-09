#ifndef LISP_H
#define LISP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define CODE_MAX 20000

enum {
	TYPE_NIL,
	TYPE_T,
	TYPE_INT,
	TYPE_CAR,
	TYPE_IF,
	TYPE_SETQ,
	TYPE_DEFUN,
	TYPE_STR,

	TYPE_ADD,
	TYPE_SUB,
	TYPE_MUL,
	TYPE_DIV,

	TYPE_EQ,
	TYPE_LT,
	TYPE_LE,
	TYPE_GT,
	TYPE_GE
};

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

enum {
	TOKEN_INT,
	TOKEN_STR,
	TOKEN_OPERATE,
	TOKEN_BRACE_OPEN,
	TOKEN_BRACE_CLOSE
};

typedef struct Token{
	int type;
	union{
		int num;
		char str[64];
	};
	char *input;
}Token;

struct Func;

cons_t *create_list(Token *token);
void free_cons(cons_t *c);
int compile(cons_t *, struct Func *func, int reg, int argsp);
int compile_func(cons_t *, struct Func *func, int reg, int argsp);
struct Code *add_code(int inst, int v1, int v2);

#endif

