#include "func.h"
#include "lisp.h"
#include "inst.h"
#include "memory.h"

Func *funclist = NULL;
Var global_var[GLOBAL_VAR_MAX];

Func *add_function(char *name, cons_t *arg, Code *code, int code_index)
{
	Func *f = (Func *)low_malloc(sizeof(Func));
	f->name = name;
	f->arg = arg;
	f->code = code;
	f->code_index = code_index;
	// count
	f->arg_count = 0;
	while(arg != NULL){
		f->arg_count++;
		arg = arg->cdr;
	}
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

int add_var(char *name)
{
	int n = get_var(name);
	if(n != -1){
		return n;
	}
	int i;
	for(i=0; i<GLOBAL_VAR_MAX; i++){
		if(global_var[i].name == NULL){
			global_var[i].name = name;
			global_var[i].value = 0;
			return i;
		}
	}
	printf("add_var error\n");
	return -1;
}

int get_var(char *name)
{
	int i;
	for(i=0; i<GLOBAL_VAR_MAX; i++){
		if(global_var[i].name != NULL && strcmp(global_var[i].name, name)==0){
			return i;
		}
	}
	return -1;
}

