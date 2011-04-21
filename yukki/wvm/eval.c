#include "lisp.h"
#include "inst.h"

extern Code code[1024];
extern int code_index;

void add_code(int inst, int v1, int v2){
	code[code_index].inst = inst;
	code[code_index].v1.i = v1;
	code[code_index].v2.i = v2;
	code_index++;
}

void eval(cons_t *c){
	int op;

	switch(c->type){

	// set result to R0
	case TYPE_OPERATE:
		op = c->v.i;
		c = c->cdr;
		
		if(c->type == TYPE_INT){
			add_code(MOV_V, 0, c->v.i);
		}else if(c->type == TYPE_CAR){
			eval(c->v.car);			
		}else{
			printf("operate error 1\n");
		}
		c = c->cdr;

		while(c != NULL){
			if(c->type == TYPE_INT){
				add_code(MOV_V, 1, c->v.i);
				add_code(op, 0, 1);
			}else if(c->type == TYPE_CAR){
				add_code(PUSH, 0, 0);
				eval(c->v.car);
				add_code(POP, 1, 0);
				add_code(op, 1, 0);
				add_code(MOV_R, 0, 1);
			}else{
				printf("operate error 2\n");
			}
			c = c->cdr;
		}

		//add_code(RET, 0, 0);
		break;

	// set result to flag
	case TYPE_COMPARE:
		op = c->v.i;
		c = c->cdr;

		if(c->type == TYPE_INT){
			add_code(MOV_V, 0, c->v.i);
		}else if(c->type == TYPE_CAR){
			eval(c->v.car);			
		}else{
			printf("compare error 1\n");
		}
		c = c->cdr;

		if(c->type == TYPE_INT){
			add_code(MOV_V, 1, c->v.i);
		}else if(c->type == TYPE_CAR){
			add_code(PUSH, 0, 0);
			eval(c->v.car);
			add_code(MOV_R, 1, 0);
			add_code(POP, 0, 0);			
		}else{
			printf("compare error 1\n");
		}

		add_code(op, 0, 1);
		break;


	case TYPE_IF:
		/*c = c->cdr;
		if(c->type == TYPE_CAR){
			eval(c->v.car);
		}else if(c->type == TYPE_T){
			eval(c->cdr);
			break;
		}else if(c->type == TYPE_NIL){
			eval(c->cdr->cdr);
			break;
		}else{
			printf("if error \n");
		}
		// now code index
		index = code_index;
		code_index++;

		c = c->cdr;

		// true case
		eval(c);
		c = c->cdr;

		code[index].inst = CMP;
		code[index].v1.c = &code[index+1];
		code[index].v2.c = &code[code_index];

		// false case
		eval(c);*/
		break;


	case TYPE_CAR:
		eval(c->v.car);
		break;

	case TYPE_INT:
		add_code(MOV_V, 0, c->v.i);
		break;

	default:
		printf("error type %d\n", c->type);
	}
}

