#include "lisp.h"

extern Module *module;

Value *get_value(IRBuilder<> *builder, cons_t *cons)
{
	if(cons->type == TYPE_INT){
		const Type *t = Type::getInt32Ty(getGlobalContext());
		return ConstantInt::get(t, cons->v.i);
	}else if(cons->type == TYPE_CAR){
		return compile(builder, cons->v.car);
	}else if(cons->type == TYPE_T){
		const Type *t = Type::getInt1Ty(getGlobalContext());
		return ConstantInt::get(t, 1);
	}else if(cons->type == TYPE_NIL){
		const Type *t = Type::getInt1Ty(getGlobalContext());
		return ConstantInt::get(t, 0);
	}else if(cons->type == TYPE_STR){
		//printf("%s\n", cons->v.str);
		// local variable ?
		Function *func = builder->GetInsertBlock()->getParent();
		int i = 0;
		for(Function::arg_iterator ai = func->arg_begin(); ai != func->arg_end(); ++ai){
			Value *v = ai;
			if(ai->getNameStr().compare(cons->v.str) == 0){
				//printf("find!!\n");
				return v;	
			}
		}
		// global variable ?
		GlobalVariable *v = module->getGlobalVariable(cons->v.str);
		if(v != NULL){
			//printf("find g!!\n");
			return v->getInitializer();
		}
		printf("not found!!\n");
	}else{
		printf("error type=%d\n", cons->type);
		return NULL;
	}
}

Value *compile(IRBuilder<> *builder, cons_t *cons)
{
	if(cons->type == TYPE_OPERATE || cons->type == TYPE_COMPARE){
		int op = cons->v.i;
		cons = cons->cdr;
		Value *v = get_value(builder, cons);
		cons = cons->cdr;

		while(cons != NULL){
			Value *v2 = get_value(builder, cons);
			if(op == OP_ADD){
				v = builder->CreateAdd(v, v2, "add");
			}else if(op == OP_SUB){
				v = builder->CreateSub(v, v2, "sub");
			}else if(op == OP_MUL){
				v = builder->CreateMul(v, v2, "mul");
			}else if(op == OP_LT){
				v = builder->CreateICmpSLT(v, v2, "lt");
			}else if(op == OP_LE){
				v = builder->CreateICmpSLE(v, v2, "le");
			}else if(op == OP_GT){
				v = builder->CreateICmpSGT(v, v2, "gt");
			}else if(op == OP_GE){
				v = builder->CreateICmpSGE(v, v2, "ge");
			}else if(op == OP_EQ){
				v = builder->CreateICmpEQ(v, v2, "eq");
			}else{
				printf("operator error %d\n", op);
			}
			cons = cons->cdr;
		}
		return v;
	}else if(cons->type == TYPE_IF){
		cons = cons->cdr;
		Value *v = get_value(builder, cons);
		cons = cons->cdr;

		Function *func = builder->GetInsertBlock()->getParent();
		BasicBlock *bbt = BasicBlock::Create(getGlobalContext(), "bbt", func);
		BasicBlock *bbf = BasicBlock::Create(getGlobalContext(), "bbf");
		BasicBlock *bbm = BasicBlock::Create(getGlobalContext(), "bbm");

		builder->CreateCondBr(v, bbt, bbf);

		// emit then value
		builder->SetInsertPoint(bbt);
		Value *vt = get_value(builder, cons);
		builder->CreateBr(bbm);
		bbt = builder->GetInsertBlock();

		// emit else block
		func->getBasicBlockList().push_back(bbf);
		builder->SetInsertPoint(bbf);
		Value *vf = get_value(builder, cons->cdr);

		builder->CreateBr(bbm);
		bbf = builder->GetInsertBlock();

		// emit merge block
		func->getBasicBlockList().push_back(bbm);
		builder->SetInsertPoint(bbm);
		PHINode *phi = builder->CreatePHI(Type::getInt32Ty(getGlobalContext()), "if");
		phi->addIncoming(vt, bbt);
		phi->addIncoming(vf, bbf);
		return phi;
	}else if(cons->type == TYPE_DEFUN){
	
	}else if(cons->type == TYPE_SETQ){
	
	}else if(cons->type == TYPE_CAR){
		return compile(builder, cons->v.car);
	}else if(cons->type == TYPE_STR){
		Function *func = module->getFunction(cons->v.str);
		if(func == NULL){
			printf("function error %s\n", cons->v.str);
			return NULL;
		}
		cons = cons->cdr;

		std::vector<Value *> args;
		while(cons != NULL){
			args.push_back(get_value(builder, cons));
			cons = cons->cdr;
		}
		return builder->CreateCall(func, args.begin(), args.end(), "call");
	}
	printf("error %d\n", cons->type);
	return NULL;
}

