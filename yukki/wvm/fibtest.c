#include "inst.h"
#include <stdio.h>

#define C(x, y, z) code[index].inst=x; code[index].v1.i=y; code[index].v2.i=z; index++
#define REG_MAX 8

extern int reg[REG_MAX];

void fib_test(){
	Code code[100];
	int index = 1;

	C(MOV_V, 0, 36);
	C(MOV_V, 1, 3);		// FIB
	C(LT, 0, 1);
	
	code[index].inst = CMP;
	code[index].v1.c = &code[index+1];
	code[index].v2.c = &code[index+3];
	index++;

	C(MOV_V, 0, 1);		// L0:
	C(RET, 0, 0);
	C(MOV_R, 1, 0);		// L1:
	C(MOV_V, 2, 1);
	C(SUB, 1, 2);
	C(PUSH, 0, 0);
	C(MOV_R, 0, 1);

	//C(10, CALL, 0, 0);	//
	code[index].inst = CALL;
	code[index].v1.c = &code[2];
	index++;

	C(MOV_R, 3, 0);
	C(POP, 0, 0);
	C(MOV_R, 1, 0);
	C(MOV_V, 2, 2);
	C(SUB, 1, 2);
	C(MOV_R, 0, 1);
	C(PUSH, 3, 0);

	//C(16, CALL, 0, 0);	//
	code[index].inst = CALL;
	code[index].v1.c = &code[2];
	index++;

	C(POP, 3, 0);
	C(ADD, 0, 3);
	C(RET, 0, 0);

	exe_code(&code[1]);
}

int main(){
	int i;
	printf("WELCOME TO LOW(Lisp Of Wakamatsu) VM Version !!\n");

	fib_test();

	// print all register
	for(i=0; i<REG_MAX; i++){
		printf("reg[%d] = %d\n", i, reg[i]);
	}
	return 0;
}
