#include "lisp.h"
#include "inst.h"
#include "func.h"

//#define PRINT_INST

extern Code codelist[CODE_MAX];
extern int code_index;
extern int code_type;
extern void **g_jtable;

#ifdef PRINT_INST
static char *inst_names[] = {
		"MOV_V", "MOV_R", "MOV_S", "MOV_FLAG",
		"ADD", "ADD_V",
		"SUB", "SUB_V",	"SUB_V_R0", "SUB_V_R1",
		"MUL", "MUL_V",
		"DIV", "DIV_V", 
		"LT", "LT_V", "LE", "LE_V", "GT", "GT_V", "GE", "GE_V", "EQ", "EQ_V",
		"LD_G", "ST_G",
		"CMP", "JMP", "PUSH", "POP", "CALL", "RET",
		"PRINT_I", "PRINT_TNIL", "END"
};
#endif

Code *add_code(int inst, int v1, int v2)
{
	if(inst == SUB_V){
		if(v1 == 0){
			inst = SUB_V_R0;
		}else if(v1 == 1){
			inst = SUB_V_R1;
		}
	}

#ifdef PRINT_INST
	printf("%s %d, %d\n", inst_names[inst], v1, v2);
#endif
	Code *c = &codelist[code_index];
	codelist[code_index].instp = g_jtable[inst];
	codelist[code_index].op0 = v1;
	codelist[code_index].op1 = v2;
	code_index++;

	return c;
}

// if error exists, return 1
int compile(cons_t *c, Func *func, int reg, int argsp)
{
	if(c == NULL){
		return 1;
	}
	if(c->type == TYPE_INT){
		add_code(MOV_V, reg, c->v.i);
		code_type = TYPE_INT;
	}else if(c->type == TYPE_CAR){
		return compile_func(c->v.car, func, reg, argsp);
	}else if(c->type == TYPE_STR){
		code_type = TYPE_INT;
		if(func != NULL){
			// local variable?
			cons_t *arg = func->arg;
			int n = 0;
			while(arg != NULL){
				if(strcmp(arg->v.str, c->v.str) == 0){
					add_code(MOV_S, reg, (func->arg_count - n - 1) + argsp);
					break;
				}
				arg = arg->cdr;
				n++;
			}
			if(arg != NULL){
				return 0;
			}
		}
		// global variable?
		int n = get_var(c->v.str);
		if(n != -1){
			add_code(LD_G, reg, n);
		}else{
			printf("var not founnd\n");
			return 1;
		}
	}else if(c->type == TYPE_T){
		add_code(MOV_FLAG, 1, 0);
		code_type = TYPE_T;
	}else if(c->type == TYPE_NIL){
		add_code(MOV_FLAG, 0, 0);
		code_type = TYPE_T;
	}else{
		printf("ERROR getvalue\n");
		return 1;
	}
	return 0;
}


int optype_to_inst(int type)
{
	switch(type){
		case TYPE_ADD: return ADD;
		case TYPE_SUB: return SUB;
		case TYPE_MUL: return MUL;
		case TYPE_DIV: return DIV;
		case TYPE_EQ: return EQ;
		case TYPE_LT: return LT;
		case TYPE_LE: return LE;
		case TYPE_GT: return GT;
		case TYPE_GE: return GE;
		default: return -1;
	}
}

// set result to register[reg]
int compile_func(cons_t *c, Func *func, int reg, int argsp)
{
	if(c == NULL){
		return 1;
	}else if(c->type == TYPE_IF){
		c = c->cdr;
		if(c==NULL || compile(c, func, reg, argsp)) return 1;
		// add CMP
		Code *cmp = add_code(CMP, 0, 0);
		c = c->cdr;
		// true case
		cmp->code0 = &codelist[code_index];
		if(c==NULL || compile(c, func, reg, argsp)) return 1;
		c = c->cdr;
		if(c != NULL){
			// add jmp
			Code *jmp = add_code(JMP, 0, 0);
			// false case
			cmp->code1 = &codelist[code_index];
			if(compile(c, func, reg, argsp)) return 1;
			jmp->code0 = &codelist[code_index];
		}else{
			cmp->code1 = &codelist[code_index];
		}
		code_type = TYPE_INT;
	}else if(c->type == TYPE_DEFUN){
		c = c->cdr;
		if(c == NULL || c->type != TYPE_STR) return 1;
		char *name = c->v.str;
		c = c->cdr;
		// args
		if(c == NULL) return 1;
		cons_t *args = c->v.car;
		c = c->cdr;
		
		Func *f = add_function(name, args, &codelist[code_index], code_index);
		if(compile(c, f, 0, 0)) {
			func = NULL;
			return 1;
		}
		add_code(RET, f->arg_count, 0);
		code_type = -1;
		printf("defun %s\n", name);
	}else if(c->type == TYPE_SETQ){
		c = c->cdr;
		if(c == NULL || c->type != TYPE_STR) return 1;
		char *name = c->v.str;
		c = c->cdr;
		int n = add_var(name);
		if(compile(c, func, reg, argsp)) return 1;
		add_code(ST_G, reg, n);
	}else if(c->type == TYPE_STR){
		Func *f = get_function(c->v.str);
		if(f == NULL){
			printf("function not fount\n");
			return 1;
		}
		c = c->cdr;
		if(c == NULL) return 1;

		// push using register
		int i;
		for(i=0; i<reg; i++){
			add_code(PUSH, i, 0);
			argsp++;
		}
		// push arguments
		cons_t *arg = f->arg;
		while(arg != NULL){
			if(compile(c, func, 0, argsp)) return 1;
			add_code(PUSH, 0, 0);
			argsp++;
			arg = arg->cdr;
			c = c->cdr;
		}
		if(c != NULL) return 1;
		// add CALL
		Code *c = add_code(CALL, 0, 0);
		c->code0 = f->code;
		if(reg != 0){
			add_code(MOV_R, reg, 0);
		}
		// pop
		for(i=reg-1; i>=0; i--){
			add_code(POP, i, 0);
		}
		code_type = TYPE_INT;
	}else{
		int op = optype_to_inst(c->type);
		if(op == -1){
			printf("error operation\n");
			return 1;
		}
		c = c->cdr;
		if(c==NULL || compile(c, func, reg, argsp)) return 1;
		c = c->cdr;
		while(c != NULL){
			if(c->type == TYPE_INT){
				add_code(op+1, reg, c->v.i);	// ex)ADD -> ADD_V
			}else{
				reg++;
				if(compile(c, func, reg, argsp)) return 1;
				reg--;
				add_code(op, reg, reg+1);
			}
			c = c->cdr;
		}
		if(op == LT || op == LE || op == GT || op == GE || op == EQ){
			code_type = TYPE_T;
		}else{
			code_type = TYPE_INT;
		}
	}
	return 0;
}

