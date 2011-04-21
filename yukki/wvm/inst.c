#include <stdio.h>
#include <stdlib.h>
#include "inst.h"

#define REG_MAX 8
#define STACK_MAX 1024

int reg[REG_MAX] = {0};
int flag = 0;
int stackptr = 0;
int baseptr = 0;
int stack[STACK_MAX];

/*
void exe_code(Code *c){
	while(1){
		switch(c->inst){
		case MOV_V:
			reg[c->v1.i] = c->v2.i;
			break;

		case MOV_R:
			reg[c->v1.i] = reg[c->v2.i];
			break;

		case MOV_B:
			reg[c->v1.i] = stack[baseptr + c->v2.i];
			break;

		case ADD:
			reg[c->v1.i] += reg[c->v2.i];
			break;

		case SUB:
			reg[c->v1.i] -= reg[c->v2.i];
			break;

		case MUL:
			reg[c->v1.i] *= reg[c->v2.i];
			break;

		case DIV:
			reg[c->v1.i] /= reg[c->v2.i];
			break;

		case MOD:
			reg[c->v1.i] %= reg[c->v2.i];
			break;

		case LT:
			flag = (reg[c->v1.i] < reg[c->v2.i]) ? 1 : 0;
			break;

		case LE:
			flag = (reg[c->v1.i] <= reg[c->v2.i]) ? 1 : 0;
			break;

		case GT:
			flag = (reg[c->v1.i] > reg[c->v2.i]) ? 1 : 0;
			break;

		case GE:
			flag = (reg[c->v1.i] >= reg[c->v2.i]) ? 1 : 0;
			break;

		case EQ:
			flag = (reg[c->v1.i] == reg[c->v2.i]) ? 1 : 0;
			break;

		case CMP:
			if(flag){
				c = c->v1.c;
			}else{
				c = c->v2.c;
			}
			continue;

		case JMP:
			c = c->v1.c;
			continue;

		case CALL:
			exe_code(c->v1.c);
			break;

		case RET:
			return;

		case PUSH:
			stack[stackptr] = reg[c->v1.i];
			stackptr++;
			break;

		case POP:
			stackptr--;
			reg[c->v1.i] = stack[stackptr];
			break;

		default:
			printf("mada dekite nai! %d\n", c->inst);
			return;
		}
		c++;
	}
}*/


void exe_code(Code *c){
	const static void *jtable[] = {
		&&I_MOV_V, &&I_MOV_R, &&I_MOV_B,
		&&I_ADD, &&I_SUB, &&I_MUL, &&I_DIV, &&I_MOD,
		&&I_LT, &&I_LE, &&I_GT, &&I_GE, &&I_EQ,
		&&I_CMP, &&I_JMP,
		&&I_PUSH, &&I_POP,
		&&I_CALL, &&I_RET
	};

	goto *jtable[c->inst];

I_MOV_V:
	reg[c->v1.i] = c->v2.i;
	c++;
	goto *jtable[c->inst];

I_MOV_R:
	reg[c->v1.i] = reg[c->v2.i];
	c++;
	goto *jtable[c->inst];

I_MOV_B:
	reg[c->v1.i] = stack[baseptr + c->v2.i];
	c++;
	goto *jtable[c->inst];

I_ADD:
	reg[c->v1.i] += reg[c->v2.i];
	c++;
	goto *jtable[c->inst];

I_SUB:
	reg[c->v1.i] -= reg[c->v2.i];
	c++;
	goto *jtable[c->inst];

I_MUL:
	reg[c->v1.i] *= reg[c->v2.i];
	c++;
	goto *jtable[c->inst];

I_DIV:
	reg[c->v1.i] /= reg[c->v2.i];
	c++;
	goto *jtable[c->inst];

I_MOD:
	reg[c->v1.i] %= reg[c->v2.i];
	c++;
	goto *jtable[c->inst];

I_LT:
	flag = (reg[c->v1.i] < reg[c->v2.i]) ? 1 : 0;
	c++;
	goto *jtable[c->inst];

I_LE:
	flag = (reg[c->v1.i] <= reg[c->v2.i]) ? 1 : 0;
	c++;
	goto *jtable[c->inst];

I_GT:
	flag = (reg[c->v1.i] > reg[c->v2.i]) ? 1 : 0;
	c++;
	goto *jtable[c->inst];

I_GE:
	flag = (reg[c->v1.i] >= reg[c->v2.i]) ? 1 : 0;
	c++;
	goto *jtable[c->inst];

I_EQ:
	flag = (reg[c->v1.i] == reg[c->v2.i]) ? 1 : 0;
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
	exe_code(c->v1.c);
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
}


void print_regs(){
	int i;
	for(i=0; i<REG_MAX; i++){
		printf("reg[%d] = %d\n", i, reg[i]);
	}
}


