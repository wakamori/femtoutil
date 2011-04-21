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

enum Tokens{
	TOKEN_INT,
	TOKEN_STR,
	TOKEN_OPERATE,
	TOKEN_COMPARE,
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


cons_t *create_list(Token *token);
void eval(cons_t *);
void add_code(int inst, int v1, int v2);

#endif

