#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include"main.h"

struct Variable_Data_t* setV (char* str)
{
    Variable_Data_t* p = &Variable_Data[(str[0] * str[1]) % (sizeof(Variable_Data) / sizeof(Variable_Data[0]))];
    while (1){
        if (p->name[0] == '\0' || strcmp(p->name,str) == 0){
            strcpy (p->name, str);
            return p;
        } else if (p->next == NULL){
            p->next = (Variable_Data_t*)malloc(sizeof(Variable_Data_t));
            p->next->name[0] = '\0';
            p = p->next;
        } else {
            p = p->next;
        }
    }
}

struct Variable_Data_t* searchV (char* str)
{
    struct Variable_Data_t* p = &Variable_Data[(str[0] * str[1]) % (sizeof(Variable_Data) / sizeof(Variable_Data[0]))];
    while (1){
        if (strcmp (p->name, str) == 0){
            return p;
        } else if (p->next != NULL){
            p = p->next;
        } else {
            return NULL;
        }
    }
}

struct Function_Data_t* searchF (char* str)
{
    struct Function_Data_t* p = &Function_Data[(str[0] * str[1]) % (sizeof(Function_Data) / sizeof(Function_Data[0]))];
    while (1){
        if (strcmp (p->name, str) == 0){
            return p;
        } else if (p->next != NULL){
            p = p->next;
        } else {
            return NULL;
        }
    }
}

struct Function_Data_t* setF (char* str,int i, void* adr)
{

    Function_Data_t* p = &Function_Data[(str[0] * str[1]) % (sizeof(Function_Data) / sizeof(Function_Data[0]))];
    while (1){
        if (p->name[0] == '\0' || strcmp(p->name,str) == 0){
            strcpy (p->name, str);
            p->value = i;
            p->adr = (cons_t*)adr;
            return p;
        } else if (p->next == NULL){
            p->next = (Function_Data_t*)malloc(sizeof(Function_Data_t));
            p->next->name[0] = '\0';
            p = p->next;
        } else {
            p = p->next;
        }
    }
}
