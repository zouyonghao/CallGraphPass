#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
    class MyFunctionPass : public FunctionPass {
    public:
        static char ID;

        MyFunctionPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            outs() << "Hello: " << F.getName() << "\n";
            return true;
        }
    };
}

char MyFunctionPass::ID = 0;

static RegisterPass<MyFunctionPass> X("mymodule", "My Module Pass");

static void registerPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
    PM.add(new MyFunctionPass());
}

static RegisterStandardPasses
        RegisterTheSpindlePass(PassManagerBuilder::EP_EarlyAsPossible, registerPass);
