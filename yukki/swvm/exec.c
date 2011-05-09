#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lisp.h"
#include "inst.h"
#include "func.h"

#define REG_MAX 16
#define STACK_MAX 1024
#define CODESTACK_MAX 64

Code codelist[CODE_MAX];
int code_index = 0;
int code_type = 0;
void **g_jtable = NULL;

void exec(Code *c_arg){
	if(c_arg == NULL){
		static void *jtable[] = {
			&&I_MOV_V, &&I_MOV_R, &&I_MOV_S, &&I_MOV_FLAG,
			&&I_ADD, &&I_ADD_V,
			&&I_SUB, &&I_SUB_V,	&&I_SUB_V_R0, &&I_SUB_V_R1,
			&&I_MUL, &&I_MUL_V,
			&&I_DIV, &&I_DIV_V,
			&&I_LT, &&I_LT_V,
			&&I_LE, &&I_LE_V,
			&&I_GT, &&I_GT_V,
			&&I_GE, &&I_GE_V,
			&&I_EQ, &&I_EQ_V,
			&&I_LD_G, &&I_ST_G,
			&&I_CMP, &&I_JMP,
			&&I_PUSH, &&I_POP,
			&&I_CALL, &&I_RET,
			&&I_PRINT_I, &&I_PRINT_TNIL,
			&&I_END
		};
		g_jtable = jtable;
		return;
	}
	
	static int	stack[STACK_MAX] = {0};
	static Code *codestack[CODESTACK_MAX] = {0};
	register Code *c = c_arg;
	register int *stackptr = stack + STACK_MAX;
	register Code **codeptr = codestack;
	int reg[REG_MAX] = {0};
	int flag = 0;

	goto *c->instp;

I_MOV_V:
	reg[c->op0] = c->op1;
	c++;
	goto *c->instp;
	
I_MOV_R:
	reg[c->op0] = reg[c->op1];
	c++;
	goto *c->instp;

I_MOV_S:
	reg[c->op0] = stackptr[c->op1];
	c++;
	goto *c->instp;

I_MOV_FLAG:
	flag = c->op0;
	c++;
	goto *c->instp;

I_PUSH:
	*(--stackptr) = reg[c->op0];
	c++;
	goto *c->instp;

I_POP:
	reg[c->op0] = *(stackptr++);
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

I_LT_V:
	flag = (reg[c->op0] < c->op1) ? 1 : 0;
	c++;
	goto *c->instp;

I_LE:
	flag = (reg[c->op0] <= reg[c->op1]) ? 1 : 0;
	c++;
	goto *c->instp;

I_LE_V:
	flag = (reg[c->op0] <= c->op1) ? 1 : 0;
	c++;
	goto *c->instp;

I_GT:
	flag = (reg[c->op0] > reg[c->op1]) ? 1 : 0;
	c++;
	goto *c->instp;

I_GT_V:
	flag = (reg[c->op0] > c->op1) ? 1 : 0;
	c++;
	goto *c->instp;

I_GE:
	flag = (reg[c->op0] >= reg[c->op1]) ? 1 : 0;
	c++;
	goto *c->instp;

I_GE_V:
	flag = (reg[c->op0] >= c->op1) ? 1 : 0;
	c++;
	goto *c->instp;

I_EQ:
	flag = (reg[c->op0] == reg[c->op1]) ? 1 : 0;
	c++;
	goto *c->instp;

I_EQ_V:
	flag = (reg[c->op0] == c->op1) ? 1 : 0;
	c++;
	goto *c->instp;

I_CMP:
	c = (flag) ? c->code0 : c->code1;
	goto *c->instp;

I_JMP:
	c = c->code0;
	goto *c->instp;

I_CALL:
	*codeptr++ = c + 1;
	c = c->code0;
	goto *c->instp;

I_RET:
	stackptr += c->op0;
	c = *(--codeptr);
	goto *c->instp;

I_LD_G:
	reg[c->op0] = global_var[c->op1].value;
	c++;
	goto *c->instp;

I_ST_G:
	global_var[c->op1].value = reg[c->op0];
	c++;
	goto *c->instp;

I_DIV:
	if(reg[c->op1] == 0){
		printf("0 division error\n");
		return;
	}
	reg[c->op0] /= reg[c->op1];
	c++;
	goto *c->instp;

I_DIV_V:
	if(c->op1 == 0){
		printf("0 division error\n");
		return;
	}
	reg[c->op0] /= c->op1;
	c++;
	goto *c->instp;

I_PRINT_I:
	printf("%d\n", reg[c->op0]);
	c++;
	goto *c->instp;

I_PRINT_TNIL:
	if(flag) printf("T\n");
	else printf("NIL\n");
	c++;
	goto *c->instp;

I_END:
	return;
	//printf("%d\n", reg[0]);
}

