#include "lisp.h"
#include "inst.h"
#include "func.h"

extern Code code[1024];
extern int code_index;
extern void **g_jtable;

static int reg = 0;

Code *add_code(int inst, int v1, int v2){
	if(inst == MOV_ARG){
		if(v1 == 0){
				inst = MOV_ARG_R0;
		}else if(v1 == 1){
				inst = MOV_ARG_R1;
		}
	}

	if(inst == SUB_V){
		if(v1 == 0){
			inst = SUB_V_R0;
		}else if(v1 == 1){
			inst = SUB_V_R1;
		}
	}

	printf("%s %d, %d\n", inst_names[inst], v1, v2);
	
	Code *c = &code[code_index];
	code[code_index].instp = g_jtable[inst];
	code[code_index].op0 = v1;
	code[code_index].op1 = v2;
	code_index++;

	return c;
}

// set result to register[reg]
void compile(cons_t *c){
	int op;
	switch(c->type){
	case TYPE_OPERATE:
		op = c->v.i;
		c = c->cdr;
		compile(c);			
		c = c->cdr;
		while(c != NULL){
			if(c->type == TYPE_INT){
				add_code(op+1, reg, c->v.i);	// ex)ADD -> ADD_V
			}else{
				reg++;
				compile(c);
				reg--;
				add_code(op, reg, reg+1);
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
			add_code(op+5, reg, c->v.i);
		}else{
			reg++;
			compile(c);
			reg--;
			add_code(op, reg, reg+1);
		}
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
		int cmp_i = code_index;
		Code *cmp = add_code(CMP, 0, 0);
		c = c->cdr;

		// true case
		cmp->op0 = 1;
		compile(c);
		c = c->cdr;

		// add RET
		add_code(RET, 0, 0);

		// false case
		cmp->op1 = code_index - cmp_i;
		compile(c);
		break;

	case TYPE_DEFUN:
		reg = 0;
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
		
		add_function(name, args, &code[code_index], code_index);
		compile(c);

		add_code(RET, 0, 0);
		break;

	case TYPE_CAR:
		compile(c->v.car);
		break;
	
	// set R0
	case TYPE_INT:
		add_code(MOV_V, reg, c->v.i);
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
				add_code(PUSH_ARG, reg, n);
				arg = arg->cdr;
				c = c->cdr;
				n++;
			}
			// push using register
			int i, r=reg;
			for(i=0; i<r; i++){
				add_code(PUSH, i, 0);
			}
			// add CALL
			reg = 0;
			Code *call = add_code(CALL, f->code_index - code_index, 0);
			reg = r;
			
			if(reg != 0){
				add_code(MOV_R, reg, 0);
			}

			// pop
			for(i=r-1; i>=0; i--){
				add_code(POP, i, 0);
			}
		}else{
				// variable?
				add_code(MOV_ARG, reg, 0);
		}
		}
		break;


	default:
		printf("error type %d\n", c->type);
	}
}

