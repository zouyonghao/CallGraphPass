#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
    class MyModulePass : public ModulePass {
    public:
        static char ID;

        MyModulePass() : ModulePass(ID) {}

        virtual bool runOnModule(Module &M) { 
            outs() << "Hello: " << "\n";
            outs() << M.getName() << "\n";
            return false;
        }
    };
}

char MyModulePass::ID = 0;

static RegisterPass<MyModulePass> X("myfunction", "My Function Pass");
static void registerPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new MyModulePass());
}

static RegisterStandardPasses
    RegisterTheSpindlePass(PassManagerBuilder::EP_EarlyAsPossible, registerPass);
