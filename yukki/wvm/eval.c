#include "lisp.h"
#include "inst.h"

extern Code code[1024];
extern int code_index;

typedef struct Func{
	char *name;
	Code *code;
}Func;

Func func[64] = {0};
int func_index = 0;

Func *get_func(char *name){
	int i;
	for(i=0; i<func_index; i++){
		if(strcmp(name, func[i].name) == 0){
			return &func[i];
		}
	}
	return NULL;
}


void add_code(int inst, int v1, int v2){
	code[code_index].inst = inst;
	code[code_index].v1.i = v1;
	code[code_index].v2.i = v2;
	code_index++;
}

void compile(cons_t *c){
	int op;
	op++++;
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
			}else if(c->type == TYPE_CAR){
				add_code(PUSH, 0, 0);
				compile(c->v.car);
				add_code(MOV_R, 1, 0);
				add_code(POP, 0, 0);
				add_code(op, 0, 1);
			}else{
				printf("operate error 2\n");
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
		Code *cmp = &code[code_index];
		cmp->inst = CMP;
		code_index++;
		c = c->cdr;

		// true case
		cmp->v1.c = &code[code_index];
		compile(c);
		c = c->cdr;

		// add JMP
		Code *jmp = &code[code_index];
		jmp->inst = JMP;
		code_index++;

		// false case
		cmp->v2.c = &code[code_index];
		compile(c);

		jmp->v1.c = &code[code_index];
		break;

	case TYPE_DEFUN:
		c = c->cdr;
		if(c->type != TYPE_STR){
				printf("defun error!\n");
				break;
		}
		func[func_index].name = c->v.str;
		c = c->cdr;
		// skip args
		c = c->cdr;
		func[func_index].code = &code[code_index];
		compile(c);
		func_index++;
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
		Func *f = get_func(c->v.str);
		if(f != NULL){
			// push arguments
			c = c->cdr;
			compile(c);
			add_code(PUSH_ARG, 0, 0);

			// add CALL
			code[code_index].inst = CALL;
			code[code_index].v1.c = f->code;

			code_index++;
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

