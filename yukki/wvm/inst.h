#ifndef INST_H
#define INST_H

enum Instruction {
	MOV_V,	// MOV r, value
	MOV_R,	// MOV r, r
	MOV_B,	// MOV r, (offset from base pointer)

	// xxx r1, r2
	ADD,	// r1 += r2
	SUB,
	MUL,
	DIV,
	MOD,

	// xxx r1, value
	ADD_V,	// r1 += value
	SUB_V,
	MUL_V,
	DIV_V,
	MOD_V,

 	// xxx r1, r2
	LT,		// if(r1<r2) flag=1 else flag=0
	LE,
	GT,
	GE,
	EQ,

	CMP,	// CMP (code if flag=1), (code if flag=0)
	JMP,	// JMP (code)
	PUSH,	// PUSH r
	POP,	// POP r
	CALL,	// CALL (code)
	RET,	// RET

	PUSH_ARG,	// r, arg_n
	END
};

typedef union CodeValue{
	int i;
	int *iPtr;
	struct Code *c;
}CodeValue;

typedef struct Code{
	void *instp;
	CodeValue v1;
	CodeValue v2;
}Code;

void exe_code(Code *, int arg);
void print_regs();

#endif

