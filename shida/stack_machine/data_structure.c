#include<stdlib.h>
#include <string.h>
#include"data_structure.h"
#include"token.h"

int malloc_size;
char* malloc_ptr;
cons_t* last;
cons_t* first;
cons_t* ptr;



variable_Data_t variable_Data[100];
function_Data_t function_Data[100];

int data[STACK_MAX];
int* sp=data;
int function_size = ( sizeof( function_Data ) / sizeof( function_Data[0] ) );
int variable_size = ( sizeof( variable_Data ) / sizeof( variable_Data[0] ) );


inline void push ( int e ) {
    *(sp++) = e;
}




inline int pop ( void ){
    return *(--sp);
}





cons_t* deq(void){
    ptr=first;
    if(first->prev!=NULL){
        first=first->prev;
    }else{
        first=NULL;
        last=NULL;
    }
    return ptr;
}


void enq(cons_t* cons){
    if(last==NULL){
        first=cons;
        last=cons;
        cons->prev=NULL;
    }else{
        cons->next=last;
        last->prev=cons;
        last=cons;
        cons->prev=NULL;
    }
}
void setq(cons_t* cons1, cons_t* cons2){
    struct variable_Data_t* p;
    struct variable_Data_t* next_p;
    p=&variable_Data[ ((int)(cons2->u.c[0]) * (int)(cons2->u.c[1])) % ( variable_size ) ];
    while(1){
        if(p->name[0] == '\0'){
            strcpy(p->name,cons2->u.c);
            p->value = cons1->u.i;
            break;
        }else if(p->name == cons2->u.c){
            break;
        }else if(p->next == NULL){
            //printf("null");
            next_p = (struct variable_Data_t*)malloc(sizeof(struct variable_Data_t));
            memset( next_p, 0, sizeof(variable_Data_t));
            p->next = next_p;
            p=next_p;
        }else{
            p=p->next;
        }
    }
}
int getq(char* str){
    struct variable_Data_t* p;
    p = &variable_Data[ (str[0] * str[1]) % ( variable_size ) ];
    while(1){
        if(strcmp(p->name,str) == 0){
            return p->value;
        }else if(p->next != NULL){
            p=p->next;
        }else{
            return -1;
        }
    }
}   
void setf(void){
    int count=0;
    int nestLevel=0;
    cons_t* func_end;
    cons_t* name=deq();
    //printf("name %s\n",name->u.c);
    struct function_Data_t* p;
    struct function_Data_t* next_p;
    //hashNum=((int)('f')*(int)('i'))%(sizeof(function_Data)/sizeof(function_Data[0]));
    p=&function_Data[ ( name->u.c[0] * name->u.c[1] ) % function_size ];
    while(1){
        if(p->name[0] == '\0'){
            strcpy(p->name,name->u.c);
            deq();
            strcpy(p->arg,deq()->u.c);
            deq();
            deq();
            p->first=first;
            func_end=first;
            while(1){
                count++;
                //printf("%d\n",func_end->type);
                if(func_end->type==OPEN){
                    nestLevel++;
                }else if(func_end->type==CLOSE){
                    nestLevel--;
                }else if(func_end->type==NUM){

                }else if(func_end->type == STR && strcmp(func_end->u.c,p->arg) == 0){
                    func_end->type = ARG;
                    //printf("argument\n");
                }else if( func_end->type == STR && strcmp(func_end->u.c,p->name) == 0){
                    func_end->f = p;
                    func_end->type = FUNC;
                }
                if(nestLevel<0){
                    break;
                }
                func_end=func_end->prev;
            }
            p->last=func_end;
            break;
        }else if(strcmp(p->name,name->u.c)==0){
            break;
        }else if(p->next == NULL){
            next_p = (struct function_Data_t*)malloc(sizeof(struct function_Data_t));
            memset( next_p, 0, sizeof(variable_Data_t));
            p->next = next_p;
            p=next_p;
        }else{
            p=p->next;
        }
    }
}
struct function_Data_t* searchf(char* str){
    struct function_Data_t* p = &function_Data[ (str[0] * str[1] ) % function_size ];
    while(1){
        if(strcmp(p->name,str) == 0){
            return p;
        }else if(p->next != NULL){
            p=p->next;
        }else{
            return NULL;
        }
    }
}
void getf(char* str,int argument, struct function_Data_t* p){
    int arg;
    int back;
    cons_t* first_copy;
    cons_t* last_copy;
    read_Expression(ONCE,argument);
    arg=pop();

    first_copy=first;
    last_copy=last;

    first=p->first;
    last=p->last;

    read_Expression(CONTINUE,arg);

    first=first_copy;
    last=last_copy;

    return;
}

void* malloc_original(int size)
{
    if (size > malloc_size) {
        malloc_ptr = (char*)malloc(sizeof(char) * MAXSIZE);
        malloc_size = MAXSIZE;
    }
    malloc_size -= size;
    malloc_ptr += size;
    return (void*)(malloc_ptr-size);
}
