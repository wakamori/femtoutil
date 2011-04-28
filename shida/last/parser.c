#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include"main.h"
#include"parser.h"
#include"generator.h"
#define ERROR printf("Error In Tokenizer\n"); return tok_error;
#define PERROR printf("Error In Parser\n"); return NULL;
AST* ParseExpression (void);
AST* ParseBlock (void);
static int CurTok;
static char* CurrentChar;
static char TokStr[200];
static int TokNum;
int GetTok (void)
{
    int ALT = 1;
    TokNum = 0;
    int TokSize = 0;
    if (*CurrentChar == '\0'){
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
        if ((*CurrentChar) != '(' && (*CurrentChar) != ')' && (*CurrentChar) != ' '){
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
        if (*CurrentChar != '(' && *CurrentChar != ')' && *CurrentChar != ' '){
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
        CurrentChar++;
        return tok_gte;
    } else if (*CurrentChar == '>' && *(CurrentChar + 1) == '=' && (*(CurrentChar + 1) == '(' || *(CurrentChar + 1) == ' ')){
        CurrentChar++;
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

AST* ParseIf (void)
{
    AST* ret = malloc(sizeof(AST));
    ret->type = tok_if;
    ret->COND = ParseExpression();
    ret->LHS = ParseBlock();
    ret->RHS = ParseBlock();
    return ret;
}

AST* ParseDefun (void)
{
    AST* ret = malloc(sizeof(AST));
    ret->type = tok_defun;
    getNextToken();
    if (CurTok == tok_str){
        strcpy(ret->u.s,TokStr);
    } else {
        printf("error in defun\n");
        return NULL;
    }

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

AST* ParseNumber (void)
{
    AST* ret;
    ret = (AST*)malloc(sizeof(AST));
    ret->type = tok_number;
    printf("%d ",TokNum);
    ret->u.i = TokNum;
    ret->LHS = ret->RHS = NULL;
    return ret;
}

AST* ParseOperation (int Tok,AST* pRHS)
{
    AST* ret = (AST*)malloc(sizeof(AST));
    AST *LHS,*RHS;
    int OpType;
    if (pRHS == NULL){
        printf("plus ");
        getNextToken(); // eat operator
        OpType = CurTok;
        getNextToken();
        LHS = ParseExpression();
        getNextToken();
        RHS = ParseExpression();

    } else {
        OpType = Tok;
        LHS = pRHS;
        RHS = ParseExpression();
    }
    switch(OpType){
        case tok_plus:
        case tok_minus:
        case tok_mul:
        case tok_div:
        case tok_lt:
        case tok_lte:
        case tok_gt:
        case tok_gte:
        case tok_eq:
        if (LHS == NULL || RHS == NULL){
                PERROR
            }
            getNextToken();
            if (CurTok == tok_number || CurTok == tok_open){
                //if( CurTok == tok_number){
                //    printf("%d    ",LHS->i);
                //}
                ret->type = OpType;
                ret->LHS = LHS;
                ret->RHS = ParseOperation(OpType, RHS);
            } else if (CurTok == tok_close){
                //printf("%d %d\n",LHS->i,RHS->i);
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
        return ret;
    } else if (CurTok == tok_open){
        ret = ParseOperation(-1, NULL);
        //getNextToken(); //eat ')'
        return ret;
    } else {
        PERROR
    }
}

AST* ParseBlock (void)
{
    char *p = CurrentChar;
    getNextToken();
    if (CurTok == tok_open){
        getNextToken();
        if (CurTok == tok_defun){
            return ParseDefun();
        } else if (CurTok == tok_setq){
            return ParseSetq();
        } else if (CurTok == tok_if){
            return ParseIf();
        } else {
            CurrentChar = p;
            getNextToken();
            return ParseExpression();
        }
    }
}

void ParseProgram (void)
{
    //generate(ParseBlock());
}

/*int main (void)
{
    int ans;
    strcpy(str, "(+(+ 1 1) (+ 2 2) (+ 3 3) )");
    CurrentChar = str;
    ParseBlock();
    exit(1);
    while (1){
        ans = GetTok();
        if (ans == tok_eof){
            printf("eof\n");
            break;
        } else if (ans == tok_number){
            printf("%d\n", TokNum);
        } else if (ans == tok_str){
            printf("%s\n", TokStr);
        } else if (ans == tok_gt){
            printf("<\n");
        } else if (ans == tok_if){
            printf("if\n");
        } else if (ans == tok_open){
            printf("(\n");
        } else if (ans == tok_close){
            printf(")\n");
        } else if (ans == tok_plus){
            printf("+\n");
        } else if (ans == tok_minus){
            printf("-\n");
        } else if (ans == tok_error){
            return 1;
        } else if (ans == tok_defun){
            printf("defun\n");
        }
    }
}*/
