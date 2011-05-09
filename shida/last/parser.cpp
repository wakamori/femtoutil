#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include"main.h"
#define ERROR /*printf("Error In Tokenizer\n");*/ return tok_error;
#define ARGERROR if (ArgCount[ArgIndex] > p->value)printf("Too many "); else printf("Too few "); printf("arguments given to function\n");  return NULL;
#define PERROR FreeAST(ret);return NULL;
#define AR 3
#define LR 10
AST* ParseExpression (void);
AST* ParseBlock (void);
void FreeAST (AST*);
static int CurTok;
static char* CurrentChar;
static char* TokStr;
static int TokNum;
static int ArgIndex;
static unsigned int LengthRatio;
static int ArgsRatio = AR;
static char** Args;
int GetTok (void)
{
    char* TokTemp = NULL;
    int ALT = 1;
    TokNum = 0;
    unsigned int TokSize = 0;
    if (*CurrentChar == '\0' || *CurrentChar == '\n'){
        return tok_eof;
    }
    while (isspace(*CurrentChar) || *CurrentChar == ','){
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
        if ((*CurrentChar) != '(' && (*CurrentChar) != ')' && (*CurrentChar) != ' ' && (*CurrentChar) != '\n' && *CurrentChar != ','){
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
            if (TokSize >= LengthRatio - 1){
                LengthRatio *= 2;
                TokTemp = (char*)calloc(LengthRatio,sizeof(char));
                strncpy(TokTemp,TokStr,LengthRatio);
                free(TokStr);
                TokStr = TokTemp;
            }
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
        } else if (strcmp(TokStr,"T") == 0){
            return tok_T;
        } else if (strcmp(TokStr,"nil") == 0){
            return tok_nil;
        }
        if (*CurrentChar != '(' && *CurrentChar != ')' && *CurrentChar != ' ' && (*CurrentChar) != '\n' && *CurrentChar != ','){
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
    } else if (*CurrentChar == '>' && *(CurrentChar + 1) == '=' && (*(CurrentChar + 2) == '(' || *(CurrentChar + 2) == ' ')){
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
    char** ArgsTemp;
    int count = 0, count1 = 0;
    AST* ret = (AST*)malloc(sizeof(AST));
    ret->LHS = ret->RHS = ret->COND = NULL;
    ret->s = NULL;
    getNextToken(); //eat '('
    if (CurTok != tok_open){
        printf("wrong prototype\n");
        FreeAST(ret);
        return NULL;
    }
    getNextToken();
    while (CurTok != tok_close){
        if (CurTok != tok_str){
            FreeAST(ret);
            return NULL;
        }
        if (count == ArgsRatio){
            ArgsRatio *= 2;
            ArgsTemp = (char**)calloc(ArgsRatio,sizeof(char*));
            for (count1 = 0; count1 < ArgsRatio / 2; count1++){
                ArgsTemp[count1] = Args[count1];
            }
            free(Args);
            Args = ArgsTemp;
        }
        Args[count] = (char*)malloc(LengthRatio);
        strncpy(Args[count],TokStr,LengthRatio);
        count++;
        getNextToken();
    }
    ret->type = tok_arg;
    ret->i = count;
    return ret;

}

AST* ParseIf (void)
{
    getNextToken(); //eat '(' or number
    AST* ret = (AST*)malloc(sizeof(AST));
    ret->LHS = ret->RHS = ret->COND = NULL;
    ret->s = NULL;
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
    AST* ret = (AST*)malloc(sizeof(AST));
    ret->LHS = ret->RHS = ret->COND = NULL;
    ret->s = NULL;
    ret->type = tok_defun;
    getNextToken();
    if (CurTok == tok_str){
        ret->s = (char*)malloc(sizeof(char) * LengthRatio);
        strncpy(ret->s,TokStr,LengthRatio);
    } else {
        printf("Error in defun\n");
        FreeAST(ret);
        return NULL;
    }
    ret->LHS = ParseArgument();
    if (ret->LHS == NULL){
        FreeAST(ret);
        return NULL;
    }
    p = setF(ret->s, ret->LHS->i, NULL, LengthRatio);
    ret->RHS = ParseBlock();
    if (ret->RHS == NULL){
        FreeAST(ret);
        return NULL;
    }
    getNextToken();
    if (CurTok == tok_close){
        for (i = 0; i < ArgsRatio; i++){
            free(Args[i]);
            Args[i] = NULL;
        }
        return ret;
    }
    PERROR
}

AST* ParseSetq (void)
{
    AST* ret = (AST*)malloc(sizeof(AST));
    ret->LHS = ret->RHS = ret->COND = NULL;
    ret->s = NULL;
    ret->type = tok_setq;
    getNextToken();
    if (CurTok == tok_str){
        ret->s = (char*)malloc(LengthRatio);
        strncpy(ret->s, TokStr, LengthRatio);
    } else {
        printf("error in setq\n");
        FreeAST(ret);
        return NULL;
    }
    getNextToken();
    ret->LHS = ParseExpression();
    if (ret->LHS == NULL){
        printf("error in setq\n");
        FreeAST(ret);
        return NULL;
    }
    setV(ret->s,LengthRatio);
    return ret;
}

AST* ParseVariable (void)
{
    AST* ret = NULL;
    int i;
    for(i = 0; i < ArgsRatio; i++){
        if (Args[i] != NULL && strcmp(Args[i],TokStr) == 0){
            ret = (AST*)malloc(sizeof(AST));
            ret->LHS = ret->RHS = ret->COND = NULL;
            ret->s = NULL;
            ret->type = tok_arg;
            ret->i = i;
            return ret;
        }
    }
    if (searchV(TokStr) != NULL){
        ret = (AST*)malloc(sizeof(AST));
        ret->LHS = ret->RHS = ret->COND = NULL;
        ret->s = NULL;
        ret->type = tok_valiable;
        ret->s = (char*)malloc(LengthRatio);
        strncpy(ret->s, TokStr, LengthRatio);
        return ret;
    } else {
        printf("valiable not found\n");
        FreeAST(ret);
        return NULL;
    }
}

AST* ParseNumber (void)
{
    AST* ret = NULL;
    ret = (AST*)malloc(sizeof(AST));
    ret->LHS = ret->RHS = ret->COND = NULL;
    ret->s = NULL;
    ret->type = tok_number;
    ret->i = TokNum;
    ret->LHS = ret->RHS = NULL;
    return ret;
}

AST* ParseT (void)
{
    AST* ret = NULL;
    ret = (AST*)malloc(sizeof(AST));
    ret->LHS = ret->RHS = ret->COND = NULL;
    ret->s = NULL;
    ret->type = tok_T;
    ret->LHS = ret->RHS = NULL;
    return ret;
}

AST* ParseNil (void)
{
    AST* ret = NULL;
    ret = (AST*)malloc(sizeof(AST));
    ret->RHS = ret->LHS = ret->COND = NULL;
    ret->s = NULL;
    ret->type = tok_nil;
    ret->LHS = ret->RHS = NULL;
    return ret;
}

AST* ParseOperation (int Tok,AST* pRHS)
{
    static int ArgCount[20];
    char* fname;
    AST* ret = (AST*)malloc(sizeof(AST));
    ret->LHS = ret->RHS = ret->COND = NULL;
    ret->s = NULL;
    AST *LHS,*RHS;
    Function_Data_t* p;
    int OpType;
    if (pRHS == NULL){
        //printf("plus ");
        getNextToken(); // eat operator
        OpType = CurTok;
        if (CurTok == tok_str && (*CurrentChar == ' ' || *CurrentChar == ')' || *CurrentChar == ',')){
            p = searchF(TokStr);
            if (p != NULL){
                ArgCount[ArgIndex] = 0;
                fname = (char*)malloc(LengthRatio);
                strncpy(fname, TokStr,LengthRatio);
                ret->type = tok_func;
                if ( p->value == 0){
                    ret->s = fname;
                    ret->LHS = NULL;
                    ret->RHS = NULL;
                    getNextToken(); // eat ')'
                    if (CurTok == tok_close){
                        return ret;
                    } else {
                        free(fname);
                        ARGERROR
                    }
                } else if (p->value == 1){
                    getNextToken();
                    ret->s = fname;
                    ret->RHS = ParseExpression();
                    ret->LHS = NULL;
                    getNextToken(); //eat ')'
                    if (CurTok == tok_close && ret->RHS != NULL){
                        return ret;
                    } else {
                        free(fname);
                        ARGERROR
                    }
                } else {
                    getNextToken();
                    free(ret);
                    ret = NULL;
                    ArgCount[ArgIndex]++;
                    LHS = ParseExpression();
                    if (LHS != NULL){
                        getNextToken();
                        if (CurTok == tok_open || CurTok == tok_number || CurTok == tok_str){
                            RHS = ParseOperation(tok_func, LHS);
                        } else {
                            free(fname);
                            ARGERROR
                        }
                        RHS->type = tok_func;
                        if (RHS != NULL){
                            if (ArgCount[ArgIndex] == p->value){
                                RHS->s = fname;
                                return RHS;
                            } else {
                                free(fname);
                                ARGERROR
                            }
                        } else {
                            free(fname);
                            PERROR
                        }
                    } else {
                        free(fname);
                        PERROR
                    }
                }
            } else {
                PERROR
            }
        }
        getNextToken();
        LHS = ParseExpression();
        getNextToken();
        if (CurTok == tok_close){
            ret->type = OpType;
            ret->LHS = LHS;
            ret->RHS = NULL;
            return ret;
        }
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
                FreeAST(ret);
            }
            if (LHS->type == tok_nil || RHS->type == tok_nil){
                printf("nil is not a real number\n");
                FreeAST(ret);
                return NULL;
            }
            if (LHS->type == tok_T || RHS->type == tok_T){
                printf("T is not a real number\n");
                FreeAST(ret);
                return NULL;
            }
            getNextToken();
            if (CurTok == tok_number || CurTok == tok_open || CurTok == tok_str){
                ret->type = OpType;
                ret->LHS = LHS;
                if (pRHS == NULL && (OpType == tok_minus || OpType == tok_div)){
                    if (OpType == tok_minus){
                        ret->RHS = ParseOperation(tok_plus, RHS);
                    } else {
                        ret->RHS = ParseOperation(tok_mul, RHS);
                    }
                } else {
                    ret->RHS = ParseOperation(OpType, RHS);
                }
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
    AST* ret = NULL;
    if (CurTok == tok_number){
        ret = ParseNumber();
        if (ret == NULL){ PERROR }
        return ret;
    } else if (CurTok == tok_T){
        ret = ParseT();
        return ret;
    } else if (CurTok == tok_nil){
        ret = ParseNil();
        return ret;
    }else if (CurTok == tok_str){
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
    AST* ret = NULL;
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
    } else if (CurTok == tok_T){
        ret = ParseT();
        return ret;
    } else if (CurTok == tok_nil){
        ret = ParseNil();
        return ret;
    }else if (CurTok == tok_str){
        ret = ParseVariable();
        return ret;
    }else {
        return NULL;
    }
    return NULL;
}

int ParseProgram (void)
{
    ArgsRatio = AR;
    LengthRatio = LR;
    Args = (char**)calloc(ArgsRatio,sizeof(char*));
    TokStr = (char*)calloc(LengthRatio,sizeof(char*));
    AST* ret = NULL;
    CurrentChar = str;

    getNextToken();
    if (CurTok == tok_eof){
        return 1;
    }

    CurrentChar = str;
    ret = ParseBlock();

    int i = 0;
    for (; i < ArgsRatio; i++) {
        free(Args[i]);
        Args[i] = NULL;
    }
    free(Args);
    getNextToken();
    free(TokStr);
    if (ret != NULL && CurTok == tok_eof ){
        GenerateProgram(ret);
        return 0;
    }
    printf("Syntax Error\n");
    return 1;
}

void FreeAST (AST* ast)
{
    if (ast == NULL){
        return;
    }
    if (ast->LHS != NULL){
        FreeAST(ast->LHS);
        ast->LHS = NULL;
    }
    if (ast->COND != NULL){
        FreeAST(ast->COND);
        ast->COND = NULL;
    }
    if (ast->RHS != NULL){
        FreeAST(ast->RHS);
        ast->RHS = NULL;
    }
    free(ast->s);
    ast->s = NULL;
    free(ast);
}
