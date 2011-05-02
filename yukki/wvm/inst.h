#ifndef INST_H
#define INST_H


static char *inst_names[] = {
		"MOV_V", "MOV_R", "MOV_B",
		"MOV_ARG_R0", "MOV_ARG_R1",
		"ADD", "ADD_V",
		"SUB", "SUB_V",	"SUB_V_R0", "SUB_V_R1",
		"MUL", "MUL_V",
		"DIV", "DIV_V", 
		"MOD", "MOD_V",
		"LT", "LE", "GT", "GE", "EQ",
		"LT_V",
		"CMP", "JMP", "PUSH", "POP", "CALL", "RET", "PUSH_ARG", "END"
};

enum Instruction {
	
	MOV_V,	// MOV r, value
	MOV_R,	// MOV r, r
	MOV_ARG,	// MOV r, (offset from base pointer)

	MOV_ARG_R0,
	MOV_ARG_R1,

	ADD,		// r1 += r2
	ADD_V,	// r1 += value

	SUB,
	SUB_V,
	SUB_V_R0,
	SUB_V_R1,

	MUL,
	MUL_V,

	DIV,
	DIV_V,

	MOD,
	MOD_V,

 	// xxx r1, r2
	LT,		// if(r1<r2) flag=1 else flag=0
	LE,
	GT,
	GE,
	EQ,

	LT_V,	// r1 < value

	CMP,	// CMP (code if flag=1), (code if flag=0)
	JMP,	// JMP (code)
	PUSH,	// PUSH r
	POP,	// POP r
	CALL,	// CALL (code)
	RET,	// RET

	PUSH_ARG,	// r, arg_n
	END
};

typedef struct Code{
	void *instp;
	long op0;
	long op1;
}Code;

void exe_code(Code *);
void print_regs();

#endif

