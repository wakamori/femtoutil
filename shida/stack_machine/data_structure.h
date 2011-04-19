#include<stdio.h>
#ifndef TREE_H
#define TREE_H
#define ONCE 1
#define CONTINUE 0
#define MAXSIZE 2000000
#define STACK_MAX 100000
inline  int pop( void );
inline void push( int );
enum eTYPE{ PLUS, MINUS, MUL, DIV, GT, GTE, LT, LTE, EQ, SETQ, NUM,  T, nil, IF, OPEN, CLOSE, DEFUN, STR, ARG, STATIC, FUNC};
typedef union{
    int i;
    char* c;
}union_t;
typedef struct cons_t{
    enum eTYPE type;
    union_t u;
    struct cons_t* next;
    struct cons_t* prev;
    void* f;
}cons_t;
typedef struct function_Data_t{
    struct function_Data_t* next;
    char name[40];
    char arg[40];
    cons_t* first;
    cons_t* last;
}function_Data_t;

cons_t* cons;

typedef struct variable_Data_t{
    struct variable_Data_t* next;
    char name[40];
    int value;
}variable_Data_t;

void enq(cons_t*);
void enq_func(cons_t*,cons_t**,cons_t**);
cons_t* deq_func();
struct cons_t* deq(void);
void setq(cons_t*,cons_t*);
int* getq(char*);


struct function_Data_t* searchf(char*);
void getf(char*,int,struct function_Data_t*);
void* malloc_original(int size);

union_t* escape_Data(function_Data_t*);
void recover_Data(function_Data_t*,union_t*);

extern variable_Data_t variable_Data[100];
extern function_Data_t function_Data[100];


extern char* malloc_ptr;
extern cons_t* last;
extern cons_t* first;

#endif /*TREE_H*/
