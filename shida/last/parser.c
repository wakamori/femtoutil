#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include"main.h"
#include"hash.h"
#include"parser.h"
#include"generator.h"
#define ERROR printf("Error In Tokenizer\n"); return tok_error;
#define ARGERROR if (ArgCount[ArgIndex] > p->value)printf("Too many "); else printf("Too few "); printf("arguments given to function\n");  return NULL;
#define PERROR return NULL;
#define ARGSIZE 10
AST* ParseExpression (void);
AST* ParseBlock (void);
static int CurTok;
static char* CurrentChar;
static char TokStr[200];
static int TokNum;
static int ArgIndex;
static char Args[ARGSIZE][20];
int GetTok (void)
{
    int ALT = 1;
    TokNum = 0;
    int TokSize = 0;
    if (*CurrentChar == '\0' || *CurrentChar == '\n'){
        return tok_eof;
    }
    while (isspace(*CurrentChar)){
        CurrentChar++;
    }
    if (isdigit(*CurrentChar) || (*CurrentChar == '-' && isdigit(*(CurrentChar + 1)))){
        if (*CurrentChar == '-'){
            ALT = -1;
            CurrentChar++;
        }
        while (isdigit(*CurrentChar)){
            TokNum = 10 * TokNum + (*CurrentChar - 48);
            CurrentChar++;
        }
        TokNum *= ALT;
        if ((*CurrentChar) != '(' && (*CurrentChar) != ')' && (*CurrentChar) != ' ' && (*CurrentChar) != '\n'){
            ERROR
        } else {
            return tok_number;
        }
    }
    if (isalpha(*CurrentChar)){
        while (isalnum(*CurrentChar)){
            TokStr[TokSize] = *CurrentChar;
            TokSize++;
            CurrentChar++;
        }

        TokStr[TokSize] = '\0';
        if (strcmp(TokStr,"defun") == 0){
            if(*CurrentChar != ' '){
                ERROR
            } else {
                return tok_defun;
            }
        } else if (strcmp(TokStr,"if") == 0){
            if (*CurrentChar != ' '){
                ERROR
            } else {
                return tok_if;
            }
        } else if (strcmp(TokStr,"setq") == 0){
            if (*CurrentChar != ' '){
                ERROR
            } else {
                return tok_setq;
            }
        }
        if (*CurrentChar != '(' && *CurrentChar != ')' && *CurrentChar != ' ' && (*CurrentChar) != '\n'){
            ERROR
        } else {
            return tok_str;
        }
    }
    if (*CurrentChar == '(' && !isdigit(*(CurrentChar + 1))){
        CurrentChar++;
        return tok_open;
    } else if (*CurrentChar == ')'){
        CurrentChar++;
        return tok_close;
    } else if (*CurrentChar == '<' && (*(CurrentChar + 1) == '(' || *(CurrentChar + 1) == ' ')){
        CurrentChar++;
        return tok_gt;
    } else if (*CurrentChar == '>' && (*(CurrentChar + 1) == '(' || *(CurrentChar + 1) == ' ')){
        CurrentChar++;
        return tok_lt;
    } else if (*CurrentChar == '<' && *(CurrentChar + 1) == '=' && (*(CurrentChar + 2) == '(' || *(CurrentChar + 2) == ' ')){
        CurrentChar += 2;
        return tok_gte;
    } else if (*CurrentChar == '>' && *(CurrentChar + 1) == '=' && (*(CurrentChar + 1) == '(' || *(CurrentChar + 1) == ' ')){
        CurrentChar += 2;
        return tok_lte;
    } else if (*CurrentChar == '=' && (*(CurrentChar + 1) == '(' || *(CurrentChar + 1) == ' ')){
        CurrentChar++;
        return tok_eq;
    } else if (*CurrentChar == '+' && (*(CurrentChar + 1) == '(' || *(CurrentChar + 1) == ' ')){
        CurrentChar++;
        return tok_plus;
    } else if (*CurrentChar == '-' && (*(CurrentChar + 1) == '(' || *(CurrentChar + 1) == ' ')){
        CurrentChar++;
        return tok_minus;
    } else if (*CurrentChar == '*' && (*(CurrentChar + 1) == '(' || *(CurrentChar + 1) == ' ')){
        CurrentChar++;
        return tok_mul;
    } else if (*CurrentChar == '/' && (*(CurrentChar + 1) == '(' || *(CurrentChar + 1) == ' ')){
        CurrentChar++;
        return tok_div;
    }

    if( *CurrentChar == '\0'){
        return tok_eof;
    }
    ERROR

} 

void getNextToken (void)
{
    CurTok = GetTok();
}

AST* ParseArgument (void)
{
    int count = 0;
    AST* ret = (AST*)malloc(sizeof(AST));
    getNextToken(); //eat '('
    if (CurTok != tok_open){
        printf("error in defun\n");
        ret->type = -1;
        return ret;
    }
    getNextToken();
    while (CurTok != tok_close){
        if (CurTok != tok_str){
            ret->type = -1;
            return ret;
        }
        strcpy(Args[count],TokStr);
        count++;
        getNextToken();
    }
    ret->type = tok_arg;
    ret->u.i = count;
    return ret;

}

AST* ParseIf (void)
{
    getNextToken(); //eat '('
    AST* ret = malloc(sizeof(AST));
    ret->type = tok_if;
    ret->COND = ParseExpression();
    ret->LHS = ParseBlock();
    ret->RHS = ParseBlock();
    if (ret->LHS == NULL || ret->RHS == NULL || ret->COND == NULL ){
        PERROR
    }
    getNextToken();
    if (CurTok != tok_close){
        PERROR
    }
    return ret;
}

AST* ParseDefun (void)
{
    Function_Data_t* p = NULL;
    int i = 0;
    AST* ret = malloc(sizeof(AST));
    ret->type = tok_defun;
    getNextToken();
    if (CurTok == tok_str){
        strcpy(ret->u.s,TokStr);
    } else {
        printf("error in defun\n");
        return NULL;
    }
    ret->LHS = ParseArgument();
    p = setF(ret->u.s, ret->LHS->u.i, NULL);
    if ((signed int)ret->LHS->type == -1){
        free(ret->LHS);
        free(ret);
        //p->name[0] = '\0';
        return NULL;
    }
    ret->RHS = ParseBlock();
    if (ret->RHS == NULL)
        //p->name[0] = '\0';
        return NULL;
    getNextToken();
    if (CurTok == tok_close){
        for (i = 0; i < ARGSIZE; i++){
            Args[i][0] = '\0';
        }
        return ret;
    }
    //p->name[0] = '\0';
    PERROR
}

AST* ParseSetq (void)
{
    AST* ret = malloc(sizeof(AST));
    ret->type = tok_setq;
    getNextToken();
    if (CurTok == tok_str){
        strcpy(ret->u.s, TokStr);
    } else {
        printf("error in setq\n");
        return NULL;
    }
    getNextToken();
    ret->LHS = ParseExpression();
    if (ret->LHS == NULL){
        printf("error in setq\n");
        return NULL;
    }
    return ret;
}

AST* ParseVariable (void)
{
    AST* ret;
    int i;
    for(i = 0; i < ARGSIZE; i++){
        if (strcmp(Args[i],TokStr) == 0){
            ret = (AST*)malloc(sizeof(AST));
            ret->type = tok_arg;
            ret->u.i = i;
            return ret;
        }
    }
    if (searchV(TokStr) != NULL){
        ret = (AST*)malloc(sizeof(AST));
        ret->type = tok_valiable;
        strcpy(ret->u.s, TokStr);
        return ret;
    } else {
        printf("valiable not found\n");
        return NULL;
    }
}

AST* ParseNumber (void)
{
    AST* ret;
    ret = (AST*)malloc(sizeof(AST));
    ret->type = tok_number;
    //printf("%d ",TokNum);
    ret->u.i = TokNum;
    ret->LHS = ret->RHS = NULL;
    return ret;
}

AST* ParseOperation (int Tok,AST* pRHS)
{
    static int ArgCount[20];
    char fname[20];
    AST* ret = (AST*)malloc(sizeof(AST));
    AST *LHS,*RHS;
    Function_Data_t* p;
    int OpType;
    if (pRHS == NULL){
        //printf("plus ");
        getNextToken(); // eat operator
        OpType = CurTok;
        if (CurTok == tok_str && (*CurrentChar == ' ' || *CurrentChar == ')')){
            p = searchF(TokStr);
            if (p != NULL){
                ArgCount[ArgIndex] = 0;
                strcpy(fname, TokStr);
                ret->type = tok_func;
                if ( p->value == 0){
                    strcpy(ret->u.s,fname);
                    ret->LHS = NULL;
                    ret->RHS = NULL;
                    getNextToken(); // eat ')'
                    if (CurTok == tok_close){
                        return ret;
                    } else {
                        ARGERROR
                    }
                } else if (p->value == 1){
                    getNextToken();
                    strcpy(ret->u.s,fname);
                    ret->RHS = ParseExpression();
                    ret->LHS = NULL;
                    getNextToken(); //eat ')'
                    if (CurTok == tok_close && ret->RHS != NULL){
                        return ret;
                    } else {
                        ARGERROR
                    }
                } else {
                    getNextToken();
                    free(ret);
                    ArgCount[ArgIndex]++;
                    LHS = ParseExpression();
                    if (LHS != NULL){
                        getNextToken();
                        if (CurTok == tok_open || CurTok == tok_number || CurTok == tok_str){
                            RHS = ParseOperation(tok_func, LHS);
                        } else {
                            ARGERROR
                        }
                        RHS->type = tok_func;
                        if (RHS != NULL){
                            if (ArgCount[ArgIndex] == p->value){
                                strcpy(RHS->u.s,fname);
                                return RHS;
                            } else {
                                ARGERROR
                            }
                        } else {
                            PERROR
                        }
                    } else {
                        PERROR
                    }
                    PERROR
                }
            } else {
                PERROR
            }
        }
        getNextToken();
        LHS = ParseExpression();
        getNextToken();
        RHS = ParseExpression();

    } else {
        OpType = Tok;
        LHS = pRHS;
        ArgCount[ArgIndex]++;
        RHS = ParseExpression();
    }
    switch(OpType){
        case tok_plus:case tok_minus:case tok_mul:case tok_div:
        case tok_lt:case tok_lte:case tok_gt:case tok_gte:
        case tok_eq:case tok_func:
            if (LHS == NULL || RHS == NULL){
                return NULL;
            }
            if (LHS->type == tok_nil || RHS->type == tok_nil){
                printf("nil is not a real number\n");
                return NULL;
            }
            if (LHS->type == tok_T || RHS->type == tok_T){
                printf("T is not a real number\n");
            }
            getNextToken();
            if (CurTok == tok_number || CurTok == tok_open || CurTok == tok_str){
                ret->type = OpType;
                ret->LHS = LHS;
                ret->RHS = ParseOperation(OpType, RHS);
                if (ret->RHS == NULL) {PERROR}
            } else if (CurTok == tok_close){
                ret->LHS = LHS;
                ret->RHS = RHS;
                ret->type = OpType;
            } else {
                PERROR
            }
            return ret;
            break;

        default:
            PERROR
    }
}

AST* ParseExpression (void)
{
    AST* ret;
    if (CurTok == tok_number){
        ret = ParseNumber();
        if (ret == NULL){ PERROR }
        return ret;
    } else if (CurTok == tok_str){
        ret = ParseVariable();
        if (ret == NULL){ PERROR }
        return ret;
    } else if (CurTok == tok_open){
        ArgIndex++;
        ret = ParseOperation(-1, NULL);
        ArgIndex--;
        if (ret == NULL){ PERROR }
        return ret;
    } else {
        PERROR
    }
}

AST* ParseBlock (void)
{
    AST* ret;
    char *p = CurrentChar;
    getNextToken();
    if (CurTok == tok_open){
        getNextToken();
        if (CurTok == tok_defun){
            ret = ParseDefun();
            if (ret == NULL){ PERROR }
            return ret;
        } else if (CurTok == tok_setq){
            ret = ParseSetq();
            getNextToken();
            if (ret == NULL){ PERROR }
            return ret;
        } else if (CurTok == tok_if){
            ret = ParseIf();
            if (ret == NULL){
                PERROR
            }
            return ret;
        } else {
            CurrentChar = p;
            getNextToken();
            ret = ParseExpression();
            if (ret == NULL){ PERROR }
            return ret;
        }
    } else if (CurTok == tok_number){
        ret = ParseNumber();
        return ret;
    } else if (CurTok == tok_str){
        ret = ParseVariable();
        return ret;
    }else {
        return NULL;
    }
    return NULL;
}

int ParseProgram (void)
{
    AST* ret;
    CurrentChar = str;
    ret = ParseBlock();
    getNextToken();
    if (ret != NULL && CurTok == tok_eof ){
        GenerateProgram(ret);
        return 0;
    }
    printf("Syntax Error\n");
    return 1;
}
