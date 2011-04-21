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
	RET		// RET
};

typedef struct Code{
	enum Instruction inst;
	union{
		int i;
		struct Code *c;
		char *s;
	}v1, v2;
}Code;

void exe_code(Code *);
void print_regs();

#endif

