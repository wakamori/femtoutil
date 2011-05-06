#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include"main.h"

Function_Data_t Function_Data[100];
Variable_Data_t Variable_Data[100];
int CurrentIndex, NextIndex;
char str[200];
void** table;
Module* TheModule;
ExecutionEngine *TheExecutionEngine;
FunctionPassManager *TheFPM;
std::map<std::string,std::string> FunctionMap;


int main (int argc, char* args[])
{
    FILE* file = NULL;
    if (argc == 2){
        file = fopen(args[1],"r");
    }
    CurrentIndex = NextIndex = 0;
    InitializeNativeTarget();
    LLVMContext &Context = getGlobalContext();
    TheModule = new Module("a",Context);
    std::string ErrStr;
    TheExecutionEngine = EngineBuilder(TheModule).setErrorStr(&ErrStr).create();
    FunctionPassManager OurFPM(TheModule);
    OurFPM.add(new TargetData(*TheExecutionEngine->getTargetData()));
    OurFPM.add(createBasicAliasAnalysisPass());
    OurFPM.add(createInstructionCombiningPass());
    OurFPM.add(createReassociatePass());
    OurFPM.add(createGVNPass());
    OurFPM.add(createCFGSimplificationPass());
    OurFPM.doInitialization();

    TheFPM  = &OurFPM;
    while (1){
        CurrentIndex = NextIndex;
        if (argc == 2){
            if (fgets(str,sizeof(str),file) == NULL){
                fclose(file);
                return 0;
            }
        } else {
            printf(">>>");
            if (fgets(str,sizeof(str),stdin) != NULL){
                if (strcmp(str,"bye\n") == 0){
                    printf("bye\n");
                    exit(0);
                }
            }
        }
        if (ParseProgram() == 0){
            //eval(argc+1);
        } else {
            argc = 1;
        }
    }

}




