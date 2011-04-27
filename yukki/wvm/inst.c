#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "inst.h"

#define REG_MAX 8
#define STACK_MAX 1024

#define PROF_START()
#define PROF_STOP()

void **g_jtable = NULL;

void exe_code(Code *c_arg){
	if(c_arg == NULL){
		static void *jtable[] = {
			&&I_MOV_V, &&I_MOV_R, &&I_MOV_ARG,
			&&I_MOV_ARG_R0,
			&&I_MOV_ARG_R1,
			&&I_ADD, &&I_ADD_V,
			&&I_SUB, &&I_SUB_V,	&&I_SUB_V_R0, &&I_SUB_V_R1,
			&&I_MUL, &&I_MOV_V,
			&&I_DIV, &&I_DIV_V,
			&&I_MOD, &&I_MOD_V,
		
			&&I_LT, &&I_LE, &&I_GT, &&I_GE, &&I_EQ,
			&&I_LT_V,
			&&I_CMP, &&I_JMP,
			&&I_PUSH, &&I_POP,
			&&I_CALL, &&I_RET,
			&&I_PUSH_ARG, &&I_END
		};
		g_jtable = jtable;
		return;
	}

	int r_arg = 0;
	int arg = 0;
	int flag = 0;
	long reg[REG_MAX] = {0};
	int	stack[STACK_MAX] = {0};
	Code *codestack[64] = {0};
	register int *stackptr = stack;
	register Code **codeptr = codestack;
	register Code *c = c_arg;

	time_t t1, t2;

	goto *c->instp;

I_MOV_V:
	reg[c->op0] = c->op1;
	c++;
	goto *c->instp;

I_MOV_R:
	reg[c->op0] = reg[c->op1];
	c++;
	goto *c->instp;

I_MOV_ARG:
	reg[c->op0] = arg;
	c++;
	goto *c->instp;

I_MOV_ARG_R0:
	reg[0] = arg;
	c++;
	goto *c->instp;

I_MOV_ARG_R1:
	reg[1] = arg;
	c++;
	goto *c->instp;

I_PUSH:
	*stackptr++ = reg[c->op0];
	c++;
	goto *c->instp;

I_POP:
	reg[c->op0] = *(--stackptr);
	c++;
	goto *c->instp;

I_PUSH_ARG:
	r_arg = reg[c->op0];
	c++;
	goto *c->instp;

I_ADD:
	reg[c->op0] += reg[c->op1];
	c++;
	goto *c->instp;

I_ADD_V:
	reg[c->op0] += c->op1;
	c++;
	goto *c->instp;

I_SUB:
	reg[c->op0] -= reg[c->op1];
	c++;
	goto *c->instp;

I_SUB_V:
	reg[c->op0] -= c->op1;
	c++;
	goto *c->instp;

I_SUB_V_R0:
	reg[0] -= c->op1;
	c++;
	goto *c->instp;

I_SUB_V_R1:
	reg[1] -= c->op1;
	c++;
	goto *c->instp;

I_MUL:
	reg[c->op0] *= reg[c->op1];
	c++;
	goto *c->instp;

I_MUL_V:
	reg[c->op0] *= c->op1;
	c++;
	goto *c->instp;

I_LT:
	flag = (reg[c->op0] < reg[c->op1]) ? 1 : 0;
	c++;
	goto *c->instp;

I_LE:
	flag = (reg[c->op0] <= reg[c->op1]) ? 1 : 0;
	c++;
	goto *c->instp;

I_GT:
	flag = (reg[c->op0] > reg[c->op1]) ? 1 : 0;
	c++;
	goto *c->instp;

I_GE:
	flag = (reg[c->op0] >= reg[c->op1]) ? 1 : 0;
	c++;
	goto *c->instp;

I_EQ:
	flag = (reg[c->op0] == reg[c->op1]) ? 1 : 0;
	c++;
	goto *c->instp;

I_LT_V:
	flag = (reg[c->op0] < c->op1) ? 1 : 0;
	c++;
	goto *c->instp;

I_CMP:
	c += (flag) ? c->op0 : c->op1;
	goto *c->instp;

I_JMP:
	c += c->op0;
	goto *c->instp;

I_CALL:
	*codeptr++ = c + 1;
	*stackptr++ = arg;
	arg = r_arg;
	c += c->op0;
	goto *c->instp;

I_RET:
	c = *(--codeptr);
	arg = *(--stackptr);
	goto *c->instp;


I_DIV:
	reg[c->op0] /= reg[c->op1];
	c++;
	goto *c->instp;

I_DIV_V:
	reg[c->op0] /= c->op1;
	c++;
	goto *c->instp;

I_MOD:
	reg[c->op0] %= reg[c->op1];
	c++;
	goto *c->instp;

I_MOD_V:
	reg[c->op0] %= c->op1;
	c++;
	goto *c->instp;



I_END:
	PROF_STOP();
	fprintf(stderr, "%d\n", t2 - t1);
	printf("%d\n", reg[0]);
}


void print_regs(){
	/*int i;
	for(i=0; i<REG_MAX; i++){
		printf("reg[%d] = %d\n", i, reg[i]);
	}
	printf("flag = %d\n", flag);
*/
}

