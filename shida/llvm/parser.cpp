#include"parser.h"
#include"lexer.h"

int getNextToken(){
    return CurTok = gettok();
}
ExprAST *Error(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0;}
PrototypeAST *ErrorP(const char *Str) { Error(Str); return 0; }
FunctionAST *ErrorF(const char *Str) { Error(Str); return 0; }

static ExprAST *ParseNumberExpr() {
    ExprAST *Result = new NumberExprAST(NumVal);
    getNextToken(); // consume the number
    return Result;
}

static ExprAST *ParseParenExpr() {
    getNextToken();  // eat (.
    ExprAST *V = ParseExpression();
    if (!V) return 0;

    if (CurTok != ')')
        return Error("expected ')'");
    getNextToken();  // eat ).
    return V;
}

static ExprAST *ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;

    getNextToken();  // eat identifier.

    if (CurTok != '(') // Simple variable ref.
        return new VariableExprAST(IdName);

    // Call.
    getNextToken();  // eat (
    std::vector<ExprAST*> Args;
    if (CurTok != ')') {
        while (1) {
            ExprAST *Arg = ParsePrimary();
            if (!Arg) return 0;
            Args.push_back(Arg);

            if (CurTok == ')') break;

            if (CurTok != ',')
                return Error("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    // Eat the ')'.
    getNextToken();

    return new CallExprAST(IdName, Args);
}

static ExprAST *ParsePrimary() {
    switch (CurTok) {
        default: return Error("unknown token when expecting an expression");
        case tok_identifier: return ParseIdentifierExpr();
        case tok_number:     return ParseNumberExpr();
        case '(':            return ParseParenExpr();
                             break;
    }
}

static FunctionAST *ParseTopLevelExpr() {
    if (ExprAST *E = ParsePrimary()) {
        // Make an anonymous proto.
        PrototypeAST *Proto = new PrototypeAST("", std::vector<std::string>());
        return new FunctionAST(Proto, E);
    }
    return 0;
}

static ExprAST *ParseExpression() {
    ExprAST *LHS;
    int Tok;
    getNextToken();
    Tok = CurTok;
    switch(CurTok){
        case '+':
        case '-':
        case '*':
        case '/':
        case '<':
            LHS = ParsePrimary();
            return ParseBinOp(Tok,LHS);
        default:
            return ParsePrimary();
    }
}
static ExprAST *ParseBinOp(int op, ExprAST *LHS){

}
static FunctionAST *ParseDefinition() {
    getNextToken();  // eat def.
    PrototypeAST *Proto = ParsePrototype();
    if (Proto == 0) return 0;

    if (ExprAST *E = ParsePrimary())
        return new FunctionAST(Proto, E);
    return 0;
}
static PrototypeAST *ParsePrototype() {
    if (CurTok != tok_identifier)
        return ErrorP("Expected function name in prototype");

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return ErrorP("Expected '(' in prototype");

    // Read the list of argument names.
    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return ErrorP("Expected ')' in prototype");

    // success.
    getNextToken();  // eat ')'.

    return new PrototypeAST(FnName, ArgNames);
}

