#ifndef MAIN
#define MAIN
#define STACK_MAX 100000
#define MALLOC_SIZE 100000


enum eINSTRUCTION { PUSH_PC, PLUS, MINUS, MUL, DIV, END, GT, GTE, LT, LTE, EQ, JMP, GOTO, RETURN, ARG, POP, PUSH, PUSH_BOOL };
enum eTYPE { T = 0, nil = 1, NUM, VAL };
typedef struct cons_t{
    enum eINSTRUCTION instruction;
    void* instruction_ptr;
    union{
        int i;
        char* c;
        struct cons_t* adr;
    }op[2];
}cons_t;

cons_t* pc;
void* pc_next;

void *malloc_ptr;
extern int malloc_size;

typedef struct value_t{
    enum eTYPE type;
    union{
        int i;
        char* c;
    }u;
}value_t;

typedef struct function_Data_t{
    char name[40];
    char arg[40];
    struct function_Data_t* next;
    int arg_count;
    cons_t* adr;
}function_Data_t;
function_Data_t function_Data[100];

cons_t* stack_adr[STACK_MAX];
int stack_arg[STACK_MAX];
value_t stack_value[STACK_MAX];
value_t* sp_value;
int* sp_arg;
cons_t** sp_adr;
#endif /*MAIN*/
