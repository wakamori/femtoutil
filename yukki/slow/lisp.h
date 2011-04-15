#ifndef LISP_H
#define LISP_H

#include <stdio.h>
#include <stdlib.h>

#define TYPE_INT 1
#define TYPE_CAR 2
#define TYPE_OPERATE 3
#define TYPE_NIL 4
#define TYPE_T 5
#define TYPE_IF 6
#define TYPE_DEFUN 7

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

cons_t *compile(char *input);
cons_t *eval(cons_t *list);

#endif

