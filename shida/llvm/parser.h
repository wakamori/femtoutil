#ifndef PARSE
#define PARSE
#include"ast.h"
#include"lexer.h"
int getNextToken(); 
static int CurTok;
static ExprAST *ParseNumberExpr();
static ExprAST *ParseParenExpr();
static ExprAST *ParseIdentifierExpr();
static ExprAST *ParsePrimary();
static FunctionAST *ParseTopLevelExpr();
static FunctionAST *ParseDefinition();
static ExprAST *ParseExpression();
static PrototypeAST *ParsePrototype();
#endif
