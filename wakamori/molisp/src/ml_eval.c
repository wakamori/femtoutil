#include "molisp.h"

#define OP_SWITCH_LOOP(pc) goto *((pc++)->adr)
#define OP_CASE(op)        op
#define OP_BREAK           goto *((pc++)->adr)
#define OP_SWITCH_LOOP_END

/**
 * replace inst to label
 */
void replaceLabel(code_t *code, const void *inst[])
{
	op_t op;
	do {
		op = code->op;
		code->adr = inst[op];
	} while (++code, op != RET);
}

/**
 * run VM
 */
void ml_eval(vm_t *vm, code_t *pc)
{
	static const void *inst[] = {
		&&RET,
		&&PUSH,
		&&PUSH0,
		&&PUSH1,
		&&PUSH2,
		&&LDA,
		&&DEC,
		&&INC,
		&&ADD,
		&&SUB,
		&&MUL,
		&&DIV,
		&&NOP
	};
	replaceLabel(pc, inst);

	OP_SWITCH_LOOP(pc);
	OP_CASE(RET):
		DBG("stack[%d] = %d", vm->sp, vm->stack[vm->sp]);
		return;
	OP_CASE(PUSH):
		vm->stack[++vm->sp] = (pc - 1)->opr;
		OP_BREAK;
	OP_CASE(PUSH0):
		vm->stack[++vm->sp] = 0;
		OP_BREAK;
	OP_CASE(PUSH1):
		vm->stack[++vm->sp] = 1;
		OP_BREAK;
	OP_CASE(PUSH2):
		vm->stack[++vm->sp] = 2;
		OP_BREAK;
	OP_CASE(LDA):
		vm->stack[++vm->sp] = (pc - 1)->opr;
		OP_BREAK;
	OP_CASE(DEC):
		vm->stack[vm->sp] -= 1;
		OP_BREAK;
	OP_CASE(INC):
		vm->stack[vm->sp] += 1;
		OP_BREAK;
	OP_CASE(ADD):
		vm->stack[vm->sp - 1] = vm->stack[vm->sp - 1] + vm->stack[vm->sp];
		vm->sp--;
		OP_BREAK;
	OP_CASE(SUB):
		vm->stack[vm->sp - 1] = vm->stack[vm->sp - 1] - vm->stack[vm->sp];
		vm->sp--;
		OP_BREAK;
	OP_CASE(MUL):
		vm->stack[vm->sp - 1] = vm->stack[vm->sp - 1] * vm->stack[vm->sp];
		vm->sp--;
		OP_BREAK;
	OP_CASE(DIV):
		vm->stack[vm->sp - 1] = vm->stack[vm->sp - 1] / vm->stack[vm->sp];
		vm->sp--;
		OP_BREAK;
	OP_CASE(NOP):
		OP_BREAK;
	OP_SWITCH_LOOP_END;
}
