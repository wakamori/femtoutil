#include"main.h"
#include <stdlib.h>
#include <stdio.h>

IRBuilder<> Builder(getGlobalContext());

Value* Generate (AST* ast, int i, char* str);
int TempIndex;
char* null = NULL;


Value* GenerateOperation(AST* ast, int i)
{
    Value* LHS = Generate(ast->LHS, i, null);
    Value* RHS = NULL;
    if (ast->RHS == NULL){
        return LHS;
    } else {
        RHS = Generate(ast->RHS, i, null);
        switch (ast->type){
            case tok_plus: return Builder.CreateAdd(LHS, RHS, "addtmp"); break;
            case tok_minus: return Builder.CreateSub(LHS, RHS, "subtmp");break;
            case tok_mul: return Builder.CreateMul(LHS, RHS, "multmp");break;
                          //case tok_div: return Builder.CreateDiv(LHS, RHS, "divtmp");
            case tok_gt:
                          LHS = Builder.CreateICmpULT(LHS, RHS, "cmptmp");
                          return Builder.CreateUIToFP(LHS, Type::getDoubleTy(getGlobalContext()),"booltmp");
                          break;


        }
    }
    return NULL;
}


Value* GenerateNumber (AST* ast){
    return ConstantInt::get(Type::getInt32Ty(getGlobalContext()), ast->u.i);
}
Value* GenerateIf (AST* ast, int i, char* str){
    Value* CondV = Generate(ast->COND, i, str);
    CondV = Builder.CreateFCmpONE(CondV, 
            ConstantFP::get(getGlobalContext(),APFloat(0.0)),
            "ifcond");

    Function *TheFunction = Builder.GetInsertBlock()->getParent();
    BasicBlock *ThenBB = BasicBlock::Create(getGlobalContext(), "then", TheFunction);
    BasicBlock *ElseBB = BasicBlock::Create(getGlobalContext(), "else");
    BasicBlock *MergeBB = BasicBlock::Create(getGlobalContext(), "ifcont");
    Builder.CreateCondBr(CondV, ThenBB, ElseBB);

    Builder.SetInsertPoint(ThenBB);

    Value *ThenV = Generate(ast->LHS, i, str);

    Builder.CreateBr(MergeBB);
    ThenBB = Builder.GetInsertBlock();

    TheFunction->getBasicBlockList().push_back(ElseBB);
    Builder.SetInsertPoint(ElseBB);

    Value* ElseV = Generate(ast->RHS, i, str);

    Builder.CreateBr(MergeBB);
    ElseBB = Builder.GetInsertBlock();

    TheFunction->getBasicBlockList().push_back(MergeBB);
    Builder.SetInsertPoint(MergeBB);
    PHINode *PN = Builder.CreatePHI(Type::getInt32Ty(getGlobalContext()), 
            "iftmp");

    PN->addIncoming(ThenV, ThenBB);
    PN->addIncoming(ElseV, ElseBB);
    return PN;



}
Value* GenerateSetq (AST* ast,int i){
    Variable_Data_t* p = NULL;
    Generate (ast->LHS, i, null);
    memory[NextIndex].instruction = SETQ;
    memory[NextIndex].instruction_ptr = table[memory[NextIndex].instruction];
    memory[NextIndex].op[0].adr = (cons_t*)p;
    NextIndex++;
    return NULL;
}

Value* GenerateVariable (AST* ast)
{
    printf("%s", ast->u.s);
    Value *V = NamedValues[ast->u.s];
    return V;
}

Value* GenerateDefun (AST* ast)
{
    std::vector<const Type*> Arguments(ast->Args.size(),Type::getInt32Ty(getGlobalContext()));
    FunctionType *FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()),Arguments, false);
    Function *F = Function::Create(FT, Function::ExternalLinkage, ast->u.s, TheModule);

    unsigned Idx = 0;
    for (Function::arg_iterator AI = F->arg_begin(); Idx != ast->Args.size(); ++AI, ++Idx) {
        AI->setName(ast->Args[Idx]);

        NamedValues[ast->Args[Idx]] = AI;
    }
    BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
    Builder.SetInsertPoint(BB);
    Value *RetVal = Generate(ast->RHS, 0, null);
    NamedValues.clear();
    Builder.CreateRet(RetVal);
    verifyFunction(*F);
    TheFPM->run(*F);
    //F->dump();
    return NULL;
}


Value* GenerateArg (AST* ast)
{
    //printf("%s\n", ast->u.s);
    Value* V = NamedValues[ast->u.s];
    return V;
}

Value* GenerateFunc (AST* ast, int i)
{
    Function *CalleeF = TheModule->getFunction(ast->u.s);
    if (CalleeF == 0 || CalleeF->arg_size() != ast->args->size()){
        printf("error\n");
        return NULL;
    }
    std::vector<Value*> ArgsV;
    Value* temp;
    for(unsigned count = 0, e = ast->args->size(); count != e; ++count){
        temp = Generate(ast->args->at(count),i,null);
        ArgsV.push_back(temp);
    }
    return Builder.CreateCall(CalleeF, ArgsV.begin(), ArgsV.end(), "calltmp");
}

Value* Generate (AST* ast, int i,char* str)
{
    switch(ast->type){
        case tok_plus:case tok_mul:case tok_minus:case tok_div:
        case tok_gt:case tok_gte:case tok_lt:case tok_lte:
        case tok_eq:
            return GenerateOperation(ast, i);
            break;

        case tok_number:
            return GenerateNumber(ast);
            break;

        case tok_if:
            return GenerateIf (ast, i, str);
            break;

        case tok_setq:
            return GenerateSetq (ast, i);
            break;

        case tok_valiable:
            return GenerateVariable (ast);
            break;

        case tok_defun:
            return GenerateDefun (ast);
            break;

        case tok_arg:
            return GenerateArg (ast);
            break;
        case tok_func:
            return GenerateFunc (ast, i);
            break;

        default:
            break;
    }
    return NULL;
}

void GenerateProgram (AST* ast)
{
    Function *F;

    const char* c = "";

    std::vector<const Type*> Arguments(0,Type::getInt32Ty(getGlobalContext()));
    FunctionType *FT = FunctionType::get(Type::getInt32Ty(getGlobalContext()),Arguments, false);
    F = Function::Create(FT, Function::ExternalLinkage, c, TheModule);
    BasicBlock *BB = BasicBlock::Create(getGlobalContext(), "entry", F);
    Builder.SetInsertPoint(BB);


    Value *RetVal = Generate(ast, 0, null);
    if (RetVal != NULL){
    Builder.CreateRet(RetVal);
    verifyFunction(*F);
    TheFPM->run(*F);
    void *FPtr = TheExecutionEngine->getPointerToFunction(F);
    int (*FP)() = (int (*)())(intptr_t)FPtr;
    //F->dump();
    printf("%d\n",FP());
    }
}
