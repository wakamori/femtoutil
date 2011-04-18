#include "lisp.h"
#include "list.h"

var_t *global_vars = NULL;
func_t *funcs = NULL;

cons_t eval(cons_t *c, var_t *local_vars){
	cons_t p;
	if(c->type == TYPE_OPERATE){
		int op = c->v.i;
		int n  = eval(c->cdr, local_vars).v.i;
		c = c->cdr->cdr;

		while(c != NULL){
			p = eval(c, local_vars);
			c = c->cdr;

			if(p.type == TYPE_INT){
				switch(op){
#define CASE(x, c) case c: n = n x p.v.i; break;
				CASE(+, '+');
				CASE(-, '-');
				CASE(*, '*');
				CASE(/, '/');

				case '=':
					if(n == p.v.i){
						p.type = TYPE_T;
					}else{
						p.type = TYPE_NIL;
					}
					return p;

				case '<':
					if(n < p.v.i){
						p.type = TYPE_T;
					}else{
						p.type = TYPE_NIL;
					}
					return p;

				case '>':
					if(n > p.v.i){
						p.type = TYPE_T;
					}else{
						p.type = TYPE_NIL;
					}
					return p;

				default:
					printf("operate error!\n");
				}
			}else{
				printf("type error!!\n");
			}
		}
		p.type = TYPE_INT;
		p.v.i = n;
		return p;
	}else if(c->type == TYPE_IF){
		p = eval(c->cdr, local_vars);

		if(p.type != TYPE_NIL){
			return eval(c->cdr->cdr, local_vars);
		}else{
			return eval(c->cdr->cdr->cdr, local_vars);
		}
	}else if(c->type == TYPE_SETQ){
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
	}else if(c->type == TYPE_DEFUN){
		c = c->cdr;
		funcs = set_func(funcs, c->v.str, c->cdr->v.car, c->cdr->cdr->v.car);
		printf("defun %s\n", funcs->name);
	}else if(c->type == TYPE_STR){
		func_t *f = get_func(funcs, c->v.str);
		var_t *v;

		if(f != NULL){

			cons_t *args = f->args;
			var_t *locals = NULL;
			c = c->cdr;
			//printf("func %s\n", f->name);

			while(args != NULL){
				cons_t p = eval(c, local_vars);
				locals = set_var_value(locals, args->v.str, p.v.i);

				//printf("local %s=%d\n", args->v.str, p.v.i);

				args = args->cdr;
				c = c->cdr;
			}
			return eval(f->car, locals);
		}else{
			// variable
			v = get_var_value(local_vars, c->v.str);
			if(v == NULL){
				v = get_var_value(global_vars, c->v.str);
			}
			if(v == NULL){
				printf("var get error\n");
				return p;
			}
			p.type = TYPE_INT;
			p.v.i = v->value;
		}
	}else if(c->type == TYPE_INT){
		return *c;
	}else if(c->type == TYPE_CAR){
		return eval(c->v.car, local_vars);
	}else{
		printf("error type %d\n", c->type);
	}
	return p;
}

