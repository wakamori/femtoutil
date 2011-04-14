#ifndef LISP_H
#define LISP_H

#include <stdio.h>
#include <stdlib.h>

#define TYPE_NULL 0
#define TYPE_INT 1
#define TYPE_LIST 2
#define TYPE_PLUS 3
#define TYPE_MINUS 4
#define TYPE_MULTI 5
#define TYPE_DIV 6
#define TYPE_IF 7

struct _Cons;

typedef union _Data {
	int i;
	float f;
	struct _Cons *car;
}Data;

typedef struct _Cons{
	int type;
	union _Data data;
	struct _Cons *cdr;
}Cons;	


#define TOKEN_NULL 0
#define TOKEN_INT 1
#define TOKEN_STR 2

typedef struct _Token{
	int type;
	int num;
	char str[64];
	char *input;
}Token;

Cons eval(Cons *list);

#endif

