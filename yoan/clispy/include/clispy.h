
#ifndef _LISPY_H_
#define _LISPY_H_ 1

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define BUFLINE 256
#define HASHSIZE 127

typedef enum {
	T_NULL = 0,
	T_OP_PARENTHS,
	T_CL_PARENTHS,
	T_INT,
	T_FLOAT,
	T_STR,
	T_VAR,
	T_BOOL,
	T_OPR,
	T_FUNC
} Type_t;

typedef enum {
	OP_NULL,
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_GT,
	OP_LT,
	OP_GTE,
	OP_LTE,
	OP_IF,
	OP_SETQ,
	OP_DEFUN,
	OP_MOD
} Opt_t;

typedef enum bool{
	FALSE = 0,
	TRUE
} bool;

typedef int Operator;
typedef struct Cell_t Cell_t;
typedef struct List_t List_t;

struct Cell_t {
	Type_t type;
	union {
		int inum;
		float fnum;
		char *str;
		Opt_t opr;
		Cell_t *car;
	};
	Cell_t *cdr;
};

#define CAR(c) c->car
#define TYPE(c) c->type
#define INUM(c) c->inum
#define OPR(c) c->opr
#define CDR(c) c->cdr

struct List_t{
	struct list *next;
	char *name;
	int value;
};

Cell_t* new_cell(void);
void parse(Cell_t*, char**);
void eval(Cell_t*);

#endif
