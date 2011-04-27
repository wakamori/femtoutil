#include "llvm/DerivedTypes.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Support/IRBuilder.h"
#include <cstdio>
#include <string>
#include <map>
#include <vector>
using namespace llvm;

int main(void)
{
	InitializeNativeTarget();
	LLVMContext &Context = getGlobalContext();
	Module *m = new Module("test", Context);

	//const Type *tyInt32 = Type::getInt32(Context);
	const Type *tyInt32 = Type::getFloatTy(Context);

	Function *adder = cast<Function>(m->getOrInsertFunction("add", tyInt32, tyInt32,
				(Type *)0));
	BasicBlock *bb = BasicBlock::Create(Context, "EntryBlock", adder);
	IRBuilder<> *builder = new IRBuilder<>(bb);
	ExecutionEngine *ee = EngineBuilder(m).setEngineKind(EngineKind::JIT).create();



	return 0;
}

