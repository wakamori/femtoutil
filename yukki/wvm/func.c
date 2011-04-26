#include "func.h"
#include "lisp.h"
#include "inst.h"
#include "memory.h"

Func *funclist = NULL;

Func *add_function(char *name, cons_t *arg, Code *code)
{
	Func *f = (Func *)low_malloc(sizeof(Func));
	f->name = name;
	f->arg = arg;
	f->code = code;
	f->next = funclist;
	funclist = f;
	return f;
}

Func *get_function(char *name)
{
	Func *f = funclist;
	while(f != NULL){
		if(strcmp(f->name, name) == 0){
			return f;
		}
		f = f->next;
	}
	return NULL;
}

int is_argument(Func *f, char *name)
{
	int n=0;
	cons_t *c = f->arg;
	while(c != NULL){
		if(strcmp(c->v.str, name) == 0){
			return n;
		}
		n++;
		c = c->cdr;
	}
	return -1;
}

