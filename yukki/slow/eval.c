#include "lisp.h"

// 名前候補
// KOLOLISP
// Lisp Of Wakamatsu(LOW)

cons_t *eval(cons_t *c){
	while(1){
		if(c->type == TYPE_OPERATE){
			cons_t *p1;
			cons_t *p2;
			
			if(c->cdr->type == TYPE_CAR){
				p1 = eval(c->cdr->v.car);
				p1->cdr = c->cdr->cdr;
			}else{
				p1 = c->cdr;
			}

			if(p1->cdr == NULL){
				return p1;
			}
			if(p1->cdr->type == TYPE_CAR){
				p2 = eval(p1->cdr->v.car);
				p2->cdr = p1->cdr->cdr;
			}else{
				p2 = p1->cdr;
			}
			if(p1->type == TYPE_INT && p2->type == TYPE_INT){
				switch(c->v.i){
#define CASE(op, c) case c: p2->v.i = p1->v.i op p2->v.i; break;
				CASE(+, '+');
				CASE(-, '-');
				CASE(*, '*');
				CASE(/, '/');

				case '=':
					if(p1->v.i == p2->v.i){
						c->type = TYPE_T;
					}else{
						c->type = TYPE_NIL;
					}
					c->cdr = NULL;
					return c;

				case '<':
					if(p1->v.i < p2->v.i){
						c->type = TYPE_T;
					}else{
						c->type = TYPE_NIL;
					}
					c->cdr = NULL;
					return c;

				case '>':
					if(p1->v.i > p2->v.i){
						c->type = TYPE_T;
					}else{
						c->type = TYPE_NIL;
					}
					c->cdr = NULL;
					return c;

				default:
					printf("operate error!\n");
				}
				c->cdr = p2;
			}else{
				printf("type error!!\n");
			}
		}else if(c->type == TYPE_IF){
			//mada dekite nai!!!
		}else{
			break;
		}
	}
	return c;
}

