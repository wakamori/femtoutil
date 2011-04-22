#include <stdio.h>
#include <stdlib.h>
#include "inst.h"

#define REG_MAX 8
#define STACK_MAX 1024

CodeValue reg[REG_MAX] = {0};
int flag = 0;
int stackptr = 0;
CodeValue stack[STACK_MAX];

int argstackptr = 0;
int argstack[STACK_MAX];

void exe_code(Code *c, int arg){
	const static void *jtable[] = {
		&&I_MOV_V, &&I_MOV_R, &&I_MOV_B,
		&&I_ADD, &&I_SUB, &&I_MUL, &&I_DIV, &&I_MOD,
		&&I_ADD_V, &&I_SUB_V, &&I_MUL_V, &&I_DIV_V, &&I_MOD_V,
		&&I_LT, &&I_LE, &&I_GT, &&I_GE, &&I_EQ,
		&&I_CMP, &&I_JMP,
		&&I_PUSH, &&I_POP,
		&&I_CALL, &&I_RET,
		&&I_PUSH_ARG
	};
	int r_arg;

	goto *jtable[c->inst];

I_MOV_V:
	reg[c->v1.i] = c->v2;
	c++;
	goto *jtable[c->inst];

I_MOV_R:
	reg[c->v1.i] = reg[c->v2.i];
	c++;
	goto *jtable[c->inst];

I_MOV_B:
	//reg[c->v1.i] = stack[baseptr + c->v2.i];
	reg[c->v1.i].i = arg;
	c++;
	goto *jtable[c->inst];

I_ADD:
	reg[c->v1.i].i += reg[c->v2.i].i;
	c++;
	goto *jtable[c->inst];

I_SUB:
	reg[c->v1.i].i -= reg[c->v2.i].i;
	c++;
	goto *jtable[c->inst];

I_MUL:
	reg[c->v1.i].i *= reg[c->v2.i].i;
	c++;
	goto *jtable[c->inst];

I_DIV:
	reg[c->v1.i].i /= reg[c->v2.i].i;
	c++;
	goto *jtable[c->inst];

I_MOD:
	reg[c->v1.i].i %= reg[c->v2.i].i;
	c++;
	goto *jtable[c->inst];

I_ADD_V:
	reg[c->v1.i].i += c->v2.i;
	c++;
	goto *jtable[c->inst];

I_SUB_V:
	reg[c->v1.i].i -= c->v2.i;
	c++;
	goto *jtable[c->inst];

I_MUL_V:
	reg[c->v1.i].i *= c->v2.i;
	c++;
	goto *jtable[c->inst];

I_DIV_V:
	reg[c->v1.i].i /= c->v2.i;
	c++;
	goto *jtable[c->inst];

I_MOD_V:
	reg[c->v1.i].i %= c->v2.i;
	c++;
	goto *jtable[c->inst];

I_LT:
	flag = (reg[c->v1.i].i < reg[c->v2.i].i) ? 1 : 0;
	c++;
	goto *jtable[c->inst];

I_LE:
	flag = (reg[c->v1.i].i <= reg[c->v2.i].i) ? 1 : 0;
	c++;
	goto *jtable[c->inst];

I_GT:
	flag = (reg[c->v1.i].i > reg[c->v2.i].i) ? 1 : 0;
	c++;
	goto *jtable[c->inst];

I_GE:
	flag = (reg[c->v1.i].i >= reg[c->v2.i].i) ? 1 : 0;
	c++;
	goto *jtable[c->inst];

I_EQ:
	flag = (reg[c->v1.i].i == reg[c->v2.i].i) ? 1 : 0;
	c++;
	goto *jtable[c->inst];

I_CMP:
	if(flag){
		c = c->v1.c;
	}else{
		c = c->v2.c;
	}
	goto *jtable[c->inst];

I_JMP:
	c = c->v1.c;
	goto *jtable[c->inst];

I_CALL:
	exe_code(c->v1.c, r_arg);
	c++;
	goto *jtable[c->inst];

I_RET:
	return;

I_PUSH:
	stack[stackptr] = reg[c->v1.i];
	stackptr++;
	c++;
	goto *jtable[c->inst];

I_POP:
	stackptr--;
	reg[c->v1.i] = stack[stackptr];
	c++;
	goto *jtable[c->inst];

I_PUSH_ARG:
	r_arg = reg[c->v1.i].i;
	c++;
	goto *jtable[c->inst];
}


void print_regs(){
	int i;
	for(i=0; i<REG_MAX; i++){
		printf("reg[%d] = %ld\n", i, reg[i].i);
	}
	printf("flag = %d\n", flag);
}


