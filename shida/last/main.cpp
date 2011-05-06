#include <stdio.h>
#include <mcheck.h>
#include <string.h>
#include <stdlib.h>
#include"main.h"
#define STRLEN 50
Function_Data_t Function_Data[100];
Variable_Data_t Variable_Data[100];
cons_t memory[INSTSIZE];
int CurrentIndex, NextIndex;
char* strtmp;
char* str;
void** table;
void Clean (void);


int main (int argc, char* args[])
{
    FILE* file = NULL;
    int StrSize = STRLEN;
    int StrIndex = 0;
    table = eval(1);
    mtrace();
   if (argc == 2){
        file = fopen(args[1],"r");
    }
    CurrentIndex = NextIndex = 0;
    int i;
    for (i = 0; i < (signed int)(sizeof(Function_Data)/sizeof(Function_Data[0])); i++) {
        Function_Data[i].name = NULL;
        Function_Data[i].next = NULL;
        Variable_Data[i].name = NULL;
        Variable_Data[i].next = NULL;
    }
    while (1){
        StrSize = STRLEN;
        str = (char*)malloc(StrSize);
        StrIndex = 0;
        CurrentIndex = NextIndex;
        if (argc == 2){
            while (1) {
                if ((str[StrIndex] = fgetc(file)) == EOF){
                    fclose(file);
                    free(str);
                    Clean();
                    exit(0);
                }
                if (StrIndex == StrSize - 1){
                    StrSize *= 2;
                    strtmp = (char*)malloc(StrSize);
                    strncpy(strtmp, str, StrSize);
                    free(str);
                    str = strtmp;
                }
                if (str[StrIndex] == '\n' || str[StrIndex] == '\0'){
                    str[StrIndex] = '\n';
                    str[StrIndex + 1] = '\0';
                    break;
                }
                StrIndex++;
            }

        } else {
            printf(">>>");
            while (1) {
                str[StrIndex] = fgetc(stdin);
                if (StrIndex == StrSize - 1){
                    StrSize *= 2;
                    strtmp = (char*)malloc(StrSize);
                    strncpy(strtmp, str, (StrSize / 2));
                    free(str);
                    str = strtmp;
                }
                if (str[StrIndex] == '\n' || str[StrIndex] == '\0'){
                    str[StrIndex] = '\n';
                    str[StrIndex + 1] = '\0';
                    break;
                }
                StrIndex++;
            }
        }
        if (strcmp(str,"bye\n") == 0){
            printf("bye\n");
            free(str);
            Clean();
            exit(0);
        }
        if (ParseProgram() == 0){
            eval(argc + 1);
        } else {
            argc = 1;
        }
        free(str);
    }

}


void Clean (void)
{
    Function_Data_t *tempF, *currentF;
    Variable_Data_t *tempV, *currentV;
    int i;
    for (i = 0;(unsigned int)i < (sizeof(Function_Data) / sizeof(Function_Data[0])); i++){
        free(Function_Data[i].name);
        currentF = Function_Data[i].next;
        while (1){
            if (currentF != NULL){
                tempF = currentF->next;
                free(currentF->name);
                free(currentF);
                currentF = tempF;
            } else {
                break;
            }
        }
    }
    for (i = 0;(unsigned int)i < (sizeof(Variable_Data) / sizeof(Variable_Data[0])); i++){
        free(Variable_Data[i].name);
        currentV = Variable_Data[i].next;
        while (1){
            if (currentV != NULL){
                tempV = currentV->next;
                free(currentV->name);
                free(currentV);
                currentV = tempV;
            } else {
                break;
            }
        }
    }

}

