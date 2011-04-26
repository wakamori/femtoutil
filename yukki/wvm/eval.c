#include "lisp.h"
#include "inst.h"
#include "func.h"

extern Code code[1024];
extern int code_index;
extern void **g_jtable;

char *names[] = {
		"MOV_V", "MOV_R", "MOV_B",
		"ADD", "SUB", "MUL", "DIV", "MOD",
		"ADD_V", "SUB_V", "MUL_V", "DIV_V", "MOD_V",
		"LT", "LE", "GT", "GE", "EQ",
		"CMP", "JMP", "PUSH", "POP", "CALL", "RET", "PUSH_ARG", "END"
};

void add_code(int inst, int v1, int v2){
	//code[code_index].inst = inst;
	printf("%s %d, %d\n", names[inst], v1, v2);
	code[code_index].instp = g_jtable[inst];
	code[code_index].v1.i = v1;
	code[code_index].v2.i = v2;
	code_index++;
}

Code *add_code2(int inst){
	printf("%s\n", names[inst]);
	Code*c = &code[code_index];
	c->instp = g_jtable[inst];
	code_index++;
	return c;
}

void compile(cons_t *c){
	int op;
	switch(c->type){
	// set result to R0
	case TYPE_OPERATE:
		op = c->v.i;
		c = c->cdr;
		compile(c);			
		c = c->cdr;
		while(c != NULL){
			if(c->type == TYPE_INT){
				//add_code(MOV_V, 1, c->v.i);
				//add_code(op, 0, 1);
				add_code(op+5, 0, c->v.i);	// ex)ADD -> ADD_V
			}else{
				add_code(PUSH, 0, 0);
				compile(c);
				add_code(MOV_R, 1, 0);
				add_code(POP, 0, 0);
				add_code(op, 0, 1);
			}
			c = c->cdr;
		}
		break;

	// set result to flag
	case TYPE_COMPARE:
		op = c->v.i;
		c = c->cdr;
		compile(c);			
		c = c->cdr;
		if(c->type == TYPE_INT){
			add_code(MOV_V, 1, c->v.i);
		}else if(c->type == TYPE_CAR){
			add_code(PUSH, 0, 0);
			compile(c->v.car);
			add_code(MOV_R, 1, 0);
			add_code(POP, 0, 0);			
		}else{
			printf("compare error 1\n");
		}
		add_code(op, 0, 1);
		break;

	case TYPE_IF:
		c = c->cdr;
		if(c->type == TYPE_CAR){
			compile(c->v.car);
		}else if(c->type == TYPE_T){
			compile(c->cdr);
			break;
		}else if(c->type == TYPE_NIL){
			compile(c->cdr->cdr);
			break;
		}else{
			printf("if error \n");
		}

		// add CMP
		Code *cmp = add_code2(CMP);
		c = c->cdr;

		// true case
		cmp->v1.c = &code[code_index];
		compile(c);
		c = c->cdr;

		// add JMP
		//Code *jmp = add_code2(JMP);
		add_code(RET, 0, 0);

		// false case
		cmp->v2.c = &code[code_index];
		compile(c);

		//jmp->v1.c = &code[code_index];
		break;

	case TYPE_DEFUN:
		add_code(END, 0, 0);
		c = c->cdr;
		if(c->type != TYPE_STR){
			printf("defun error!\n");
			break;
		}
		char *name = c->v.str;
		c = c->cdr;
		// args
		cons_t *args = c->v.car;
		c = c->cdr;
		
		add_function(name, args, &code[code_index]);
		compile(c);

		add_code(RET, 0, 0);
		break;

	case TYPE_CAR:
		compile(c->v.car);
		break;
	
	// set R0
	case TYPE_INT:
		add_code(MOV_V, 0, c->v.i);
		break;

	// set R0
	case TYPE_STR:
		{
		Func *f = get_function(c->v.str);
		if(f != NULL){
			c = c->cdr;
			cons_t *arg = f->arg;
			int n = 0;
			while(arg != NULL){
				compile(c);
				// push argument
				add_code(PUSH_ARG, 0, n);
				arg = arg->cdr;
				c = c->cdr;
				n++;
			}
			// add CALL
			Code *call = add_code2(CALL);
			call->v1.c = f->code;
		}else{
				// variable?
				add_code(MOV_B, 0, 0);
		}
		}
		break;


	default:
		printf("error type %d\n", c->type);
	}
}

