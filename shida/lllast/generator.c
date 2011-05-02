#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include"main.h"

void Generate (AST* ast, int i, char* str);
int TempIndex;
char* null = NULL;


void GenerateOperation(AST* ast, int i)
{
    Generate(ast->LHS, i, null);
    if(ast->RHS->type == tok_number){
        //printf("push\noperation\n");
        memory[NextIndex].instruction = ast->type + 9;
        memory[NextIndex].op[0].i = ast->RHS->u.i;
        memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
        NextIndex++;
    } else {
        Generate(ast->RHS, i, null);
        //printf("operation\n");
        memory[NextIndex].instruction = ast->type;
        memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
        NextIndex++;
    }
}

void GenerateNumber (AST* ast)
{
    //printf("push\n");
    memory[NextIndex].instruction = PUSH;
    memory[NextIndex].op[0].i = ast->u.i;
    memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
    NextIndex++;
}

void GenerateIf (AST* ast, int i, char* str)
{
    Function_Data_t* p;
    Generate(ast->COND, i, str);
    TempIndex = NextIndex;
    //printf("jmp\n");
    memory[NextIndex].instruction = JMP;
    memory[NextIndex].op[0].adr = &memory[NextIndex + 1];
    memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
    NextIndex++;
    Generate(ast->LHS , i, str);
    //printf("return or end\n");
    if (i != 1){
        memory[NextIndex].instruction = END;
    } else {
        p = searchF(str);
        if (p->value <= 1){
            memory[NextIndex].instruction = RETURN;
        } else {
            memory[NextIndex].instruction = NRETURN;
            memory[NextIndex].op[0].i = p->value;
        }
    }
    memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
    NextIndex++;
    memory[TempIndex].op[1].adr = &memory[NextIndex];
    Generate(ast->RHS, i, str);
}

void GenerateSetq (AST* ast,int i){
    Variable_Data_t* p;
    p = setV (ast->u.s);
    Generate (ast->LHS, i, null);
    //printf("setq\n");
    memory[NextIndex].instruction = SETQ;
    memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
    memory[NextIndex].op[0].adr = (cons_t*)p;
    NextIndex++;
}

void GenerateVariable (AST* ast)
{
    //printf("variable\n");
    memory[NextIndex].instruction = PUSH;
    memory[NextIndex].op[0].i = searchV(ast->u.s)->value;
    memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
    NextIndex++;
}

void GenerateDefun (AST* ast)
{
    memory[NextIndex].instruction = DEFUN;
    memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
    NextIndex++;
    Function_Data_t* p = setF (ast->u.s, ast->LHS->u.i, &memory[NextIndex]);
    memory[NextIndex - 1].op[0].c = p->name;
    free(ast->LHS);
    Generate (ast->RHS, 1,ast->u.s );
    //printf("return\n");
    if (p->value <= 1){
        memory[NextIndex].instruction = RETURN;
        memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
    } else {
        memory[NextIndex].instruction = NRETURN;
        memory[NextIndex].op[0].i = p->value;
        memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
    }
    NextIndex++;
}

void GenerateArg (AST* ast)
{
    if (ast->u.i == 0){
        memory[NextIndex].instruction = ARG;
        memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
        NextIndex++;
    } else {
        memory[NextIndex].instruction = NARG;
        memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
        memory[NextIndex].op[0].i = ast->u.i + 1;
        NextIndex++;
    }
}

void GenerateFunc (AST* ast, int i)
{
    AST* temp = ast;
    Function_Data_t* p = searchF(ast->u.s);
    int count = p->value;
    while (1){
        if (count == 0){
            memory[NextIndex].instruction = PUSH;
            memory[NextIndex].op[0].i = 0;
            memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
            NextIndex++;
            //printf("goto\n");
            memory[NextIndex].instruction = GOTO;
            memory[NextIndex].op[0].adr = p->adr;
            memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
            NextIndex++;
            break;
        } else if (count == 1){
            Generate(temp->RHS, i, null);
            //printf("goto\n");
            memory[NextIndex].instruction = GOTO;
            memory[NextIndex].op[0].adr = p->adr;
            memory[NextIndex].op[1].i = p->value;
            memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
            NextIndex++;
            break;
        } else if (count == 2){
            Generate(temp->LHS, i, null);
            Generate(temp->RHS, i, null);
            memory[NextIndex].instruction = NGOTO;
            memory[NextIndex].op[0].adr = p->adr;
            memory[NextIndex].op[1].i = p->value;
            memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
            NextIndex++;
            break;
        } else {
            Generate(temp->LHS, i, null);
            temp = temp->RHS;
            count--;
        }
    }

}

void Generate (AST* ast, int i,char* str)
{
    switch(ast->type){
        case tok_plus:case tok_mul:case tok_minus:case tok_div:
        case tok_gt:case tok_gte:case tok_lt:case tok_lte:
        case tok_eq:
            GenerateOperation(ast, i);
            break;

        case tok_number:
            GenerateNumber(ast);
            break;

        case tok_if:
            GenerateIf (ast, i, str);
            break;

        case tok_setq:
            GenerateSetq (ast, i);
            break;

        case tok_valiable:
            GenerateVariable (ast);
            break;

        case tok_defun:
            GenerateDefun (ast);
            break;

        case tok_arg:
            GenerateArg (ast);
            break;
        case tok_func:
            GenerateFunc (ast, i);
            break;

        default:
            break;
    }
    free(ast);
}

void GenerateProgram (AST* ast)
{
    Generate(ast, 0, null);
    //printf("return or end\n");
    memory[NextIndex].instruction = END;
    memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
    NextIndex++;
}
