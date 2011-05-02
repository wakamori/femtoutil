#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include"main.h"

Function_Data_t Function_Data[100];
Variable_Data_t Variable_Data[100];
cons_t memory[INSTSIZE];
int CurrentIndex, NextIndex;
char str[200];
void** table;


int main (int argc, char* args[])
{
    FILE* file = NULL;
    table = eval(1);
   if (argc == 2){
        file = fopen(args[1],"r");
    }
    CurrentIndex = NextIndex = 0;
    int i;
    for (i = 0; i < (signed int)(sizeof(Function_Data)/sizeof(Function_Data[0])); i++) {
        Function_Data[i].name[0] = '\0';
        Variable_Data[i].name[0] = '\0';
    }
    while (1){
        CurrentIndex = NextIndex;
        if (argc == 2){
            if (fgets(str,sizeof(str),file) == NULL){
                fclose(file);
                return 0;
            }
        } else {
            printf(">>>");
            fgets(str,sizeof(str),stdin);
            if (strcmp(str,"bye\n") == 0){
                printf("bye\n");
                exit(0);
            }
        }
        if (ParseProgram() == 0){
            eval(argc+1);
        } else {
            argc = 1;
        }
    }

}


