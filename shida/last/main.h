#ifndef MAIN
#define MAIN
#define STACKSIZE 100
#define INSTSIZE 100


enum TokType {  tok_number, tok_plus, tok_minus, tok_mul, tok_div, tok_eof, tok_gt, tok_gte, tok_lt, tok_lte, tok_eq, tok_if, tok_defun, tok_str, tok_setq, tok_open, tok_close, tok_error };
enum eINSTRUCTION { PUSH_PC, PLUS, MINUS, MUL, DIV, END, GT, GTE, LT, LTE, EQ, JMP, GOTO, RETURN, ARG, POP, PUSH, PUSH_BOOL };
enum eTYPE { T = 0, nil = 1, NUM, VAL };
typedef struct cons_t{
    enum eINSTRUCTION instruction;
    void* instruction_ptr;
    union{
        long int i;
        struct cons_t* adr;
    }op[2];
}cons_t;

typedef struct value_t{
    enum eTYPE type;
    long int i;
}value_t;

typedef struct AST{
    enum TokType type;
    union{
        int i;
        char s[20];
    }u;
    struct AST *LHS,*RHS,*COND;
}AST;

typedef struct function_Data_t{
    char name[40];
    char arg[40];
    struct function_Data_t* next;
    int arg_count;
    cons_t* adr;
}function_Data_t;
function_Data_t function_Data[100];
cons_t memory[INSTSIZE];
int CurrentIndex, NextIndex;
char str[200];

#endif /*MAIN*/
