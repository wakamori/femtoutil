#ifndef AST
#define AST
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
#include <vector>
class ExprAST {
    public:
          virtual ~ExprAST() {}
};

class NumberExprAST : public ExprAST {
      int Val;
    public:
        NumberExprAST(int val) : Val(val) {}
};

class VariableExprAST : public ExprAST {
      std::string Name;
    public:
        VariableExprAST(const std::string &name) : Name(name) {}
};

class BinaryExprAST : public ExprAST {
  char Op;
  ExprAST *LHS, *RHS;
public:
  BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs) 
    : Op(op), LHS(lhs), RHS(rhs) {}
};

class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<ExprAST*> Args;
public:
  CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
    : Callee(callee), Args(args) {}
};

class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;
public:
  PrototypeAST(const std::string &name, const std::vector<std::string> &args)
    : Name(name), Args(args) {}
  
};
class FunctionAST {
  PrototypeAST *Proto;
  ExprAST *Body;
public:
  FunctionAST(PrototypeAST *proto, ExprAST *body)
    : Proto(proto), Body(body) {}
  
};

#endif /*AST*/
