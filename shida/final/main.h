#ifndef MAIN
#define MAIN
#define STACKSIZE 100
#define INSTSIZE 100


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


typedef struct function_Data_t{
    char name[40];
    char arg[40];
    struct function_Data_t* next;
    int arg_count;
    cons_t* adr;
}function_Data_t;
function_Data_t function_Data[100];
cons_t hairetu[INSTSIZE];
int ipc, next_ipc;
void* ptr_base; 
#endif /*MAIN*/
