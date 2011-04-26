#include <stdio.h>
#include <stdlib.h>
#include "inst.h"

#define REG_MAX 8
#define STACK_MAX 1024

int stack[STACK_MAX];
Code *codestack[64];
Code **codeptr;

void **g_jtable = NULL;

void exe_code(Code *c_arg, int arg){
	if(c_arg == NULL){
		const static void *jtable[] = {
			&&I_MOV_V, &&I_MOV_R, &&I_MOV_B,
			&&I_ADD, &&I_SUB, &&I_MUL, &&I_DIV, &&I_MOD,
			&&I_ADD_V, &&I_SUB_V, &&I_MUL_V, &&I_DIV_V, &&I_MOD_V,
			&&I_LT, &&I_LE, &&I_GT, &&I_GE, &&I_EQ,
			&&I_CMP, &&I_JMP,
			&&I_PUSH, &&I_POP,
			&&I_CALL, &&I_RET,
			&&I_PUSH_ARG, &&I_END
		};
		g_jtable = jtable;
		return;
	}

	int reg[REG_MAX];
	int flag = 0;

	int r_arg;
	int *stackptr = stack;
	register Code *c = c_arg;

	codeptr = codestack;

	goto *c->instp;

I_MOV_V:
	reg[c->v1.i] = c->v2.i;
	c++;
	goto *c->instp;

I_MOV_R:
	reg[c->v1.i] = reg[c->v2.i];
	c++;
	goto *c->instp;

I_MOV_B:
	reg[c->v1.i] = arg;
	c++;
	goto *c->instp;

I_ADD:
	reg[c->v1.i] += reg[c->v2.i];
	c++;
	goto *c->instp;

I_SUB:
	reg[c->v1.i] -= reg[c->v2.i];
	c++;
	goto *c->instp;

I_MUL:
	reg[c->v1.i] *= reg[c->v2.i];
	c++;
	goto *c->instp;

I_DIV:
	reg[c->v1.i] /= reg[c->v2.i];
	c++;
	goto *c->instp;

I_MOD:
	reg[c->v1.i] %= reg[c->v2.i];
	c++;
	goto *c->instp;

I_ADD_V:
	reg[c->v1.i] += c->v2.i;
	c++;
	goto *c->instp;

I_SUB_V:
	reg[c->v1.i] -= c->v2.i;
	c++;
	goto *c->instp;

I_MUL_V:
	reg[c->v1.i] *= c->v2.i;
	c++;
	goto *c->instp;

I_DIV_V:
	reg[c->v1.i] /= c->v2.i;
	c++;
	goto *c->instp;

I_MOD_V:
	reg[c->v1.i] %= c->v2.i;
	c++;
	goto *c->instp;

I_LT:
	flag = (reg[c->v1.i] < reg[c->v2.i]) ? 1 : 0;
	c++;
	goto *c->instp;

I_LE:
	flag = (reg[c->v1.i] <= reg[c->v2.i]) ? 1 : 0;
	c++;
	goto *c->instp;

I_GT:
	flag = (reg[c->v1.i] > reg[c->v2.i]) ? 1 : 0;
	c++;
	goto *c->instp;

I_GE:
	flag = (reg[c->v1.i] >= reg[c->v2.i]) ? 1 : 0;
	c++;
	goto *c->instp;

I_EQ:
	flag = (reg[c->v1.i] == reg[c->v2.i]) ? 1 : 0;
	c++;
	goto *c->instp;

I_CMP:
	c = (flag) ? c->v1.c : c->v2.c;
	goto *c->instp;

I_JMP:
	c = c->v1.c;
	goto *c->instp;

I_CALL:
	//exe_code(c->v1.c, r_arg);
	//c++;
	*codeptr++ = c + 1;
	*stackptr++ = arg;
	arg = r_arg;
	c = c->v1.c;
	goto *c->instp;

I_RET:
	//return;
	c = *(--codeptr);
	arg = *(--stackptr);
	goto *c->instp;

I_PUSH:
	*stackptr++ = reg[c->v1.i];
	c++;
	goto *c->instp;

I_POP:
	reg[c->v1.i] = *(--stackptr);
	c++;
	goto *c->instp;

I_PUSH_ARG:
	r_arg = reg[c->v1.i];
	c++;
	goto *c->instp;

I_END:
	printf("%d\n", reg[0]);
	return;
}


void print_regs(){
	/*int i;
	for(i=0; i<REG_MAX; i++){
		printf("reg[%d] = %d\n", i, reg[i]);
	}
	printf("flag = %d\n", flag);
*/
}

