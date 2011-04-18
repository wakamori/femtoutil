#include<stdio.h>
#ifndef TREE_H
#define TREE_H
#define ONCE 1
#define CONTINUE 0
#define MAXSIZE 2000000

typedef union{
    int i;
    char* c;
}union_t;

enum eTYPE{ NUM, PLUS, MINUS, MUL, DIV, OPEN, CLOSE, GT, GTE, LT, LTE, EQ, T, nil, IF, SETQ, DEFUN, STR, ARG, STATIC};

typedef struct cons_t{
    enum eTYPE type;   
    enum eTYPE boolean;
    union_t u;
    struct cons_t* next;
    struct cons_t* prev;
}cons_t;

cons_t* cons;

typedef struct function_Data_t{
    struct function_Data_t* next;
    int expression_Count;
    union_t* u;
    char name[40];
    char arg[40];
    cons_t* first;
    cons_t* last;
}function_Data_t;

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
int getq(char*);
struct function_Data_t* searchf(char*);
int getf(char*,int,struct function_Data_t*);
void* malloc_original(int size);

union_t* escape_Data(function_Data_t*);
void recover_Data(function_Data_t*,union_t*);

variable_Data_t variable_Data[100];
function_Data_t function_Data[100];


extern int hashNum;
extern char* malloc_ptr;
extern cons_t* last;
extern cons_t* first;

#endif /*TREE_H*/
