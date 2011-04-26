#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define INT  0 //123...
#define CHAR 1 
#define PLUS  21 //+,-,*....
#define MINUS 22
#define MULTI 23
#define DIVID 24
#define GT   25
#define LT 26
#define GEQ   27
#define LEQ 28
#define EQ 29
#define IF 30
#define SETQ 31
#define VAR 310
#define SYMBOL 311
#define DEFUN 32 //defun func
#define FUNC 331
#define ARG1 332
#define ARG2 333
#define ARG3 334
#define ARG4 335
#define ARG5 336
#define ARG6 337
#define ARG7 338
#define ARG8 339
#define START 4 // (
#define END 5  // )
#define T 6
#define NIL 7
#define T_SIZE 50 //Size of Table
#define AS_SIZE 128
#define ARG_SIZE 8
#define ARG 111
/*----Parsing Cons Cell----*/
typedef struct cons_t {
	int type;
	union {
		struct cons_t *car;
		char *symbol;
		int ivalue;
		char *func;
	};
	struct cons_t *cdr;
} cons_t;
/*-----Variable list--------*/
struct vlist{
	int key;
	int data;
	struct vlist *next;
}vlist;
struct vlist *vtable[T_SIZE];
/*-----Function list--------*/
struct flist{
	int key;
	cons_t *aroot;
	cons_t *oroot;
	struct flist *next;
}flist;
/*------Argument Stack-------*/
int stack_num;
int as_data[AS_SIZE];

/*------Declare Function------*/
char *arg_a[ARG_SIZE];
struct a_stack *a_stack[AS_SIZE];
struct flist *ftable[T_SIZE];
int hash(char *name);
int *search(int key);
cons_t *sgmt_read(char *line, int *pos);
cons_t sgmt_eval(cons_t *cell);
void sgmt_print(cons_t *root);
void dump(cons_t *root,int depth);
void vhash(cons_t *cell);
void v_add(int key,int data);
void fhash(cons_t *cell);
void a_push(int arg);
void a_pop();
int a_get();
void error();
