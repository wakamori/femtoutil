#include "molisp.h"

/**
 * output error message
 */
void ml_error(char message[])
{
	ml_print("[error] %s\n", message);
}

/**
 * print AST
 */
void printTree(cons_t *cell, int depth)
{
	int i;
	for (i = 0; i < depth; i++)
		ml_print(" ");
	if (cell == NULL) {
		ml_print("END\n");
		return;
	}
	switch (cell->type) {
		case T_START:
			ml_print("START\n");
			printTree(cell->car, depth + 1);
			break;
		case T_INT:
			ml_print("%d\n", cell->ival);
			break;
		case T_ADD:
			ml_print("ADD\n");
			break;
		case T_SUB:
			ml_print("SUB\n");
			break;
		case T_MUL:
			ml_print("MUL\n");
			break;
		case T_DIV:
			ml_print("DIV\n");
			break;
		default:
			break;
	}
	printTree(cell->cdr, depth + 1);
}

/**
 * print bytecode
 */
void printCode(code_t *pc)
{
#define PRINT0(op)\
	case op:\
		ml_print(#op"\n");\
		break
#define PRINT1(op)\
	case op:\
		ml_print(#op" %d\n", pc->opr);\
		break
	while (1) {
		switch (pc->op) {
			case RET:
				ml_print("RET\n");
				return;
			PRINT1(PUSH);
			PRINT0(PUSH0);
			PRINT0(PUSH1);
			PRINT0(PUSH2);
			PRINT1(LDA);
			PRINT0(DEC);
			PRINT0(INC);
			PRINT0(ADD);
			PRINT0(SUB);
			PRINT0(MUL);
			PRINT0(DIV);
			PRINT0(NOP);
		}
		pc++;
	}
}
