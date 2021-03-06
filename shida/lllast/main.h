#ifndef MAIN
#define MAIN
#define STACKSIZE 1000
#define INSTSIZE 1000
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
#include <cstdio>
#include <string>
#include <iostream>
#include <map>
#include <vector>
using namespace llvm;

enum TokType {  tok_number, tok_plus, tok_minus, tok_mul, tok_div, tok_gt, tok_gte, tok_lt, tok_lte, tok_eq, tok_if, tok_defun, tok_str, tok_eof, tok_setq, tok_valiable, tok_func, tok_arg, tok_open, tok_close, tok_error, tok_nil, tok_T };
enum eINSTRUCTION { PUSH, PLUS, MINUS, MUL, DIV, GT, GTE, LT, LTE, EQ, PLUS2, MUNUS2, MUL2, DIV2, GT2, GTE2, LT2, LTE2, EQ2, END, JMP, GOTO, NGOTO, RETURN, NRETURN,  ARG, NARG, DEFUN, SETQ };
enum eTYPE { T = 0, nil = 1, NUM = 0, VAL };
typedef struct value_t{
    int type;
    int i;
}value_t;

class AST{
    //enum TokType type;
    public:
        int type;
        union{
            int i;
        }u;
        std::string s;
        std::vector<std::string> Args;
        std::vector<struct AST*>* args;
        struct AST *LHS,*RHS,*COND;
};

typedef struct Variable_Data_t{
    char name[40];
    struct Variable_Data_t* next;
    int value;
}Variable_Data_t;

typedef struct Function_Data_t{
    char name[40];
    struct Function_Data_t* next;
    int value;
}Function_Data_t;

extern Function_Data_t Function_Data[100];
extern Variable_Data_t Variable_Data[100];
extern int CurrentIndex, NextIndex;
extern char str[200];
extern void** table;
extern Module *TheModule;
extern ExecutionEngine *TheExecutionEngine;
extern FunctionPassManager *TheFPM;
extern std::map<std::string,std::string> FunctionMap;
/*hash.h*/
struct Function_Data_t* setF (char* str, int i , void* adr);
struct Variable_Data_t* setV (char* str);
struct Variable_Data_t* searchV (char* str);
struct Function_Data_t* searchF (char* str);
/*generator.h*/
static std::map<std::string, Value*> NamedValues;
void GenerateProgram (AST*);
/*parser.h*/
#define defun 1
#define normal 0
int ParseProgram();
/*eval.h*/
void** eval (int);
#endif /*MAIN*/
