#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include"main.h"
#define ERROR printf("Error In Tokenizer\n"); return tok_error;
#define ARGERROR if (ArgCount[ArgIndex] > p->value)printf("Too many "); else printf("Too few "); printf("arguments given to function\n");  return NULL;
#define PERROR return NULL;
#define ARGSIZE 10
AST* ParseExpression (void);
AST* ParseBlock (void);
static int CurTok;
static char* CurrentChar;
static std::string TokStr;
static int TokNum;
static int ArgIndex;
static char Args[ARGSIZE][20];

int GetTok (void)
{
    int ALT = 1;
    TokNum = 0;
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
        std::string temp;
        while (isalnum(*CurrentChar)){
            temp += *CurrentChar;
            CurrentChar++;
        }
        TokStr = temp;
        if (TokStr == "defun"){
            if(*CurrentChar != ' '){
                ERROR
            } else {
                return tok_defun;
            }
        } else if (TokStr == "if"){
            if (*CurrentChar != ' '){
                ERROR
            } else {
                return tok_if;
            }
        } else if (TokStr == "setq"){
            if (*CurrentChar != ' '){
                ERROR
            } else {
                return tok_setq;
            }
        } else if (TokStr == "T"){
            return tok_T;
        } else if (TokStr == "nil"){
            return tok_nil;
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

std::vector<std::string> ParseArgument (void)
{
    std::vector<std::string> ret;
    if (CurTok != tok_open){
        printf("error in defun\n");
        return ret;
    }
    getNextToken();
    while (CurTok != tok_close){
        if (CurTok != tok_str){
            return ret;
        }
        ret.push_back(*(new std::string(TokStr)));
        getNextToken();
    }
    return ret;

}

AST* ParseIf (void)
{
    getNextToken(); //eat '(' or number
    AST* ret = new AST();
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
    static std::string FCount = "a";
    int i = 0;
    AST* ret = new AST();
    ret->type = tok_defun;

    getNextToken();
    if (CurTok == tok_str){
        FCount += "a";
        FunctionMap[TokStr] = FCount;
        ret->s = FunctionMap[TokStr];
    } else {
        printf("error in defun1\n");
        return NULL;
    }

    getNextToken();
    ret->Args = ParseArgument();
    if (CurTok != tok_close){
        printf("error in defun2\n");
        return NULL;
    }
    std::vector<const Type*> Arguments(ret->Args.size(),Type::getInt32Ty(getGlobalContext()));
    FunctionType *FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()),Arguments, false);
    Function::Create(FT, Function::ExternalLinkage, ret->s, TheModule);


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
    AST* ret = new AST();
    ret->type = tok_setq;
    getNextToken();
    if (CurTok == tok_str){
        ret->s = TokStr;
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
    AST* ret = new AST();
    ret->s = TokStr;
    ret->type = tok_arg;
    return ret;
}

AST* ParseNumber (void)
{
    //printf("parsenumber %d\n",TokNum);
    AST* ret;
    ret = new AST();
    ret->type = tok_number;
    //printf("%d ",TokNum);
    ret->u.i = TokNum;
    ret->LHS = ret->RHS = NULL;
    return ret;
}

AST* ParseT (void)
{
    AST* ret;
    ret = new AST();
    ret->type = tok_T;
    ret->LHS = ret->RHS = NULL;
    return ret;
}

AST* ParseNil (void)
{
    AST* ret;
    ret = new AST();
    ret->type = tok_nil;
    ret->LHS = ret->RHS = NULL;
    return ret;
}

AST* ParseOperation (int Tok,AST* pRHS)
{
    static int ArgCount[20];
    AST* ret = new AST();
    AST *LHS,*RHS;
    AST temp;
    int OpType;
    std::string FName;
    if (pRHS == NULL){
        //printf("plus ");
        getNextToken(); // eat operator
        OpType = CurTok;
        if (CurTok == tok_str && (*CurrentChar == ' ' || *CurrentChar == ')')){
            //Function *CalleeF = TheModule->getFunction(TokStr);
            /*if (CalleeF == 0){
              printf("Unknown function referenced\n");
              return NULL;
              }*/
            ret->type = tok_func;

            std::map<std::string, std::string>::iterator itr;
            ret->s = FunctionMap[TokStr];

            getNextToken();
            ret->args = new std::vector<AST*>();
            while (CurTok != tok_close){
                if (CurTok == tok_eof){
                    PERROR
                }
                LHS = ParseExpression();
                ret->args->push_back(LHS);
                getNextToken();
            }

            Function *CalleeF = TheModule->getFunction(ret->s);
            if (CalleeF == 0 || CalleeF->arg_size() != ret->args->size()){
                if (CalleeF == 0)
                    std::cerr << "Function not fount" << std::endl;
                else
                    std::cerr << "Argument size not match" << std::endl;
                return NULL;
            }

            /*if (CalleeF->arg_size() != ret->AST.size()){
              printf("Incorrect arguments passed");
              return NULL;
              }*/
            return ret;

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
    AST* ret;
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
