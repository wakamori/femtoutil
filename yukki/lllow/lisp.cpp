#include "lisp.h"
#include "memory.h"

#define FILE_MAX (1024 * 1024)

typedef int (*func_t)();

Module *module;
static ExecutionEngine *engine;
FunctionPassManager *TheFPM;

void exec(cons_t *cons)
{
	static int count = 0;
	const Type *type = Type::getInt32Ty(getGlobalContext());

	if(cons->type == TYPE_CAR){
		cons = cons->v.car;
	}

	if(cons->type == TYPE_DEFUN){
		cons = cons->cdr;
		char *name = cons->v.str;
		printf("defun %s\n", name);
		cons = cons->cdr;
		
		std::vector<const Type *> args_type;
		std::vector<char *> args_name;
		cons_t *v = cons->v.car;
		while(v != NULL){
			printf("arg %s\n", v->v.str);
			args_type.push_back(type);
			args_name.push_back(v->v.str);
			v = v->cdr;
		}
		FunctionType *tyFunc = FunctionType::get(type, args_type, false);
		Function *func = Function::Create(tyFunc,
				GlobalValue::ExternalLinkage, name, module);

		int i = 0;
		for(Function::arg_iterator ai = func->arg_begin(); i != args_name.size(); ++i){
			ai->setName(args_name[i]);
			ai++;
		}
		cons = cons->cdr;
		//Function *func = cast<Function>(m->getOrInsertFunction(name, type, NULL));
		BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "b", func);
		IRBuilder<> *builder = new IRBuilder<>(bb);

		Value *ret_v = compile(builder, cons);
		builder->CreateRet(ret_v);
		
		verifyFunction(*func);
		TheFPM->run(*func);
		// show dump
		func->dump();
	}else if(cons->type == TYPE_SETQ){
		cons = cons->cdr;
		char *name = cons->v.str;
		cons = cons->cdr;

		char ss[256];
		sprintf(ss, "func_%d", count++);
		Function *func = cast<Function>(module->getOrInsertFunction(ss, type, NULL));
		BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "b", func);
		IRBuilder<> *builder = new IRBuilder<>(bb);
		
		Value *ret_v = get_value(builder, cons);
		builder->CreateRet(ret_v);

		verifyFunction(*func);
		TheFPM->run(*func);
		// exec
		func_t f = (func_t)engine->getPointerToFunction(func);
		int r = (*f)();	
		printf("%s = %d\n", name, r);
		
		//Value *v = ret_v;
		Constant *v = ConstantInt::get(type, r);
		Value *gv = new GlobalVariable(*module, type, false,
			GlobalValue::ExternalLinkage, v, name);

		gv->dump();
	}else{
		char ss[256];
		sprintf(ss, "func_%d", count++);
		Function *func = cast<Function>(module->getOrInsertFunction(ss, type, NULL));
		
		BasicBlock *bb = BasicBlock::Create(getGlobalContext(), "b", func);
		IRBuilder<> *builder = new IRBuilder<>(bb);

		Value *v = compile(builder, cons);
		builder->CreateRet(v);

		verifyFunction(*func);
		TheFPM->run(*func);
		// show dump
		func->dump();

		// exec
		func_t f = (func_t)engine->getPointerToFunction(func);
		int r = (*f)();	
		
		if(cons->type == TYPE_COMPARE){
			if(r){
				printf("T\n");
			}else{
				printf("nil\n");
			}
		}else{
			printf("%d\n", r);
		}
	}
}

void exe_lisp(char *input)
{
	Token token;
	cons_t *c;
	token.input = input;
	while((c = create_list(&token)) != NULL){
		exec(c);
	}
}

void open_file(char *name){
	FILE *fp;
	int size;
	char *buffer;

	if((fp = fopen(name, "r")) == NULL){
		printf("file open error:%s\n", name);
		return;
	}

	buffer = (char *)malloc(FILE_MAX);
	size = fread(buffer, 1, FILE_MAX, fp);

	fclose(fp);

	printf("read file %s\n", name);
	exe_lisp(buffer);
	free(buffer);
}
int main(int argc, char *argv[]){
	char input[256];
	int i;

	// init
	InitializeNativeTarget();
	module = new Module("lllvm_module", getGlobalContext());
	engine = EngineBuilder(module).setEngineKind(EngineKind::JIT).create();
    FunctionPassManager OurFPM(module);
    OurFPM.add(new TargetData(*engine->getTargetData()));
    OurFPM.add(createBasicAliasAnalysisPass());
    OurFPM.add(createInstructionCombiningPass());
    OurFPM.add(createReassociatePass());
    OurFPM.add(createGVNPass());
    OurFPM.add(createCFGSimplificationPass());
    OurFPM.doInitialization();

    TheFPM  = &OurFPM;
 
	puts("WELCOME TO LOW(Lisp Of Wakamatsu) VM Version !!");

	for(i=1; i<argc; i++){
		open_file(argv[i]);
		return 0;
	}

	while(1){
		printf(">>>");
		fgets(input, 256, stdin);
		if(strcmp(input, "exit\n") == 0 || strcmp(input, "bye\n") == 0){
			break;
		}
		exe_lisp(input);
	}
	return 0;
}

