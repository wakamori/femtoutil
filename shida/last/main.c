#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include"eval.h"
#include"main.h"
#include"parser.h"

int main (int argc, char* args[])
{
    FILE* file = NULL;
    table = eval(1);
        /*if (argc == 2 && (file = fopen(args[1],"r")) == NULL){
            printf("file cannot open\n");
            argc = 1;
        }*/
    //file = fopen(args[1],"r");
    if (argc == 2){
        file = fopen(args[1],"r");
    }
    CurrentIndex = NextIndex = 0;
    int i;
    for (i = 0; i < (sizeof(Function_Data)/sizeof(Function_Data[0])); i++) {
        Function_Data[i].name[0] = '\0';
        Variable_Data[i].name[0] = '\0';
    }
    //printf(">>>");
    while (1){
        CurrentIndex = NextIndex;
        if (argc == 2){
            if (fgets(str,sizeof(str),file) == NULL){
                fclose(file);
                return 0;
            }
            //printf(">>>%s",str);
        } else {
            printf(">>>");
            fgets(str,sizeof(str),stdin);
        }
        if (ParseProgram() == 0){
            eval(argc+1);
        } else {
            argc = 1;
        }
    }

}


