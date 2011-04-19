#include "lisp.h"
#include "list.h"

var_t *global_vars = NULL;
func_t *funcs = NULL;

cons_t eval(cons_t *c, var_t *local_vars){
	cons_t p;
	switch(c->type){
	case TYPE_OPERATE:{
		int (*func)(int x, int y) = c->v.func;
		int n  = eval(c->cdr, local_vars).v.i;
		c = c->cdr->cdr;
		while(c != NULL){
			p = eval(c, local_vars);
			n = func(n, p.v.i);
			c = c->cdr;
		}
		p.type = TYPE_INT;
		p.v.i = n;
		return p;
	}

	case TYPE_COMPARE:{
		int (*func)(int x, int y) = c->v.func;
		int n1 = eval(c->cdr, local_vars).v.i;
		int n2 = eval(c->cdr->cdr, local_vars).v.i;

		p.type = func(n1, n2);
		return p;
	}
		
	case TYPE_IF:
		p = eval(c->cdr, local_vars);
		if(p.type != TYPE_NIL){
			return eval(c->cdr->cdr, local_vars);
		}else{
			return eval(c->cdr->cdr->cdr, local_vars);
		}

	case TYPE_SETQ:
		c = c->cdr;
		if(c->type == TYPE_STR){
			p = eval(c->cdr, local_vars);
			if(p.type == TYPE_INT){
				const char *name = c->v.str;
				int value = p.v.i;
				global_vars = set_var_value(global_vars, name, value);
				return p;
			}else{
				printf("setq error 1");
			}
		}else{
			printf("setq error 2");
		}
		break;

	case TYPE_DEFUN:
		c = c->cdr;
		funcs = set_func(funcs, c->v.str, c->cdr->v.car, c->cdr->cdr->v.car);
	//	printf("defun %s\n", funcs->name);
		p.type = TYPE_DEFUN;
		return p;

	case TYPE_STR:{
		func_t *f = get_func(funcs, c->v.str);
		if(f != NULL){
			// function?
			cons_t *args = f->args;
			var_t *locals = NULL;
			c = c->cdr;
			while(args != NULL){
				cons_t p = eval(c, local_vars);
				locals = set_var_value(locals, args->v.str, p.v.i);

				args = args->cdr;
				c = c->cdr;
			}
			p = eval(f->car, locals);
			free_vars(locals);
		}else{
			var_t *v;
			// local variable?
			v = get_var_value(local_vars, c->v.str);
			if(v == NULL){
				// global variable?
				v = get_var_value(global_vars, c->v.str);
			}
			if(v == NULL){
				printf("var get error\n");
				return p;
			}
			p.type = TYPE_INT;
			p.v.i = v->value;
		}
		break;
	}

	case TYPE_INT:
		return *c;

	case TYPE_CAR:
		return eval(c->v.car, local_vars);
	
	default:
		printf("error type %d\n", c->type);
	}
	return p;
}

