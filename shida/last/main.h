#ifndef MAIN
#define MAIN
#define STACKSIZE 1000
#define INSTSIZE 100000


enum TokType {  tok_number, tok_plus, tok_minus, tok_mul, tok_div, tok_gt, tok_gte, tok_lt, tok_lte, tok_eq, tok_if, tok_defun, tok_str, tok_eof, tok_setq, tok_valiable, tok_func, tok_arg, tok_open, tok_close, tok_error, tok_nil, tok_T };
enum eINSTRUCTION { PUSH, PLUS, MINUS, MUL, DIV, GT, GTE, LT, LTE, EQ, PLUS2, MUNUS2, MUL2, DIV2, GT2, GTE2, LT2, LTE2, EQ2, END, JMP, GOTO, NGOTO, RETURN, NRETURN,  ARG, NARG, DEFUN, SETQ };
enum eTYPE { T = 0, nil = 1, NUM = 0, VAL };
typedef struct cons_t{
    int instruction;
    void* instruction_ptr;
    union{
        int i;
        struct cons_t* adr;
        char* c;
    }op[2];
}cons_t;

typedef struct value_t{
    int type;
    int i;
}value_t;

typedef struct AST{
    int type;
    int i;
    char* s;
    struct AST *LHS,*RHS,*COND;
}AST;

typedef struct Variable_Data_t{
    char* name;
    struct Variable_Data_t* next;
    int value;
}Variable_Data_t;

typedef struct Function_Data_t{
    char* name;
    struct Function_Data_t* next;
    int value;
    cons_t* adr;
}Function_Data_t;

extern Function_Data_t Function_Data[100];
extern Variable_Data_t Variable_Data[100];
extern cons_t memory[INSTSIZE];
extern int CurrentIndex, NextIndex;
extern char* str;
extern void** table;

/*hash.h*/
struct Function_Data_t* setF (char* str, int i , void* adr, int LengthRatio);
struct Variable_Data_t* setV (char* str, int LengthRatio);
struct Variable_Data_t* searchV (char* str);
struct Function_Data_t* searchF (char* str);
/*generator.h*/
void GenerateProgram (AST*);
/*parser.h*/
int ParseProgram();
/*eval.h*/
void** eval (int);
#endif /*MAIN*/
