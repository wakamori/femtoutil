#ifndef LISP_H
#define LISP_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "llvm/DerivedTypes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/IRBuilder.h"

using namespace llvm;

enum {
	TYPE_NIL,
	TYPE_T,
	TYPE_INT,
	TYPE_CAR,
	TYPE_OPERATE,
	TYPE_COMPARE,
	TYPE_IF,
	TYPE_SETQ,
	TYPE_DEFUN,
	TYPE_STR
};

typedef union consvalue_t{
	struct cons_t *car;
	char *str;
	int i;
}consvalue_t;

typedef struct cons_t{
	int type;
	consvalue_t v;
	struct cons_t *cdr;
}cons_t;

enum {
	TOKEN_INT,
	TOKEN_STR,
	TOKEN_OPERATE,
	TOKEN_COMPARE,
	TOKEN_BRACE_OPEN,
	TOKEN_BRACE_CLOSE
};

enum Operaters{
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_MOD,

	OP_GT,
	OP_GE,
	OP_LT,
	OP_LE,
	OP_EQ
};

typedef struct Token{
	int type;
	union{
		int num;
		char str[64];
	};
	char *input;
}Token;

cons_t *create_list(Token *token);
Value *compile(IRBuilder<> *builder, cons_t *cons);
Value *get_value(IRBuilder<> *builder, cons_t *cons);

#endif

