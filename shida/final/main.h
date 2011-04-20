#ifndef MAIN
#define MAIN
#define STACK_MAX 100000
#define MALLOC_SIZE 100000


enum eINSTRUCTION { PUSH, PUSH_PC, BOOL, PLUS, MINUS, MUL, DIV, END, GT, GTE, LT, LTE, EQ, POP };
enum eTYPE { NUM, ARG, VAL, T, nil };
typedef struct cons_t{
    enum eINSTRUCTION instruction;
    union{
        int i;
        char* c;
        struct cons_t* adr;
    }op1,op2;
}cons_t;

cons_t* pc;

void *malloc_ptr;
extern int malloc_size;

typedef struct value_t{
    enum eTYPE type;
    union{
        int i;
        char* c;
    }u;
}value_t;

value_t stack_value[STACK_MAX];
extern value_t* sp_value;
#endif /*MAIN*/
