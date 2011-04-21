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
		if(op == OP_PLUS) op = ADD;
		else if(op == OP_MINUS) op = SUB;
		else if(op == OP_MULTI) op = MUL;
		else if(op == OP_DIV) op = DIV;
		else if(op == OP_MOD) op = MOD;
		else{
				printf("operator error %d", op);
		}
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
				add_code
			}else{
				printf("operate error 2\n");
			}
			c = c->cdr;
		}

		//add_code(RET, 0, 0);
		break;

	//case TYPE_COMPARE:


		break;

	case TYPE_CAR:
		eval(c->v.car);
		break;

	default:
		printf("error type %d\n", c->type);
	}
}

