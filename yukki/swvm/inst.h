#ifndef INST_H
#define INST_H


enum Instruction{
	MOV_V,	// MOV r, value
	MOV_R,	// MOV r, r
	MOV_S,	// MOV r, (offset from stack pointer)
	MOV_FLAG,	// 0=NIL 1=T

	ADD, ADD_V,
	SUB, SUB_V, SUB_V_R0, SUB_V_R1,
	MUL, MUL_V,
	DIV, DIV_V,

 	// xxx r1, r2
	LT,	LT_V,	// if(r1<r2) flag=1 else flag=0
	LE, LE_V,
	GT, GT_V,
	GE, GE_V,
	EQ, EQ_V,

	LD_G,	// r, global_var_num
	ST_G,
	CMP,	// CMP (code if flag=1), (code if flag=0)
	JMP,	// JMP (code)
	PUSH,	// PUSH r
	POP,	// POP r
	CALL,	// CALL (code)
	RET,	// RET, pop args
	
	PRINT_I,
	PRINT_TNIL,

	END
};

typedef struct Code{
	void *instp;
	union{
		int op0;
		struct Code *code0;
	};
	union{
		int op1;
		struct Code *code1;
	};
}Code;

void exec(Code *);

#endif

