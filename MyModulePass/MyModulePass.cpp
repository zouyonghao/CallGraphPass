#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "llvm/IR/Module.h"

using namespace llvm;

namespace {
    class MyModulePass : public ModulePass {
    public:
        static char ID;

        MyModulePass() : ModulePass(ID) {}

        bool runOnModule(Module &M) override {
            outs() << "Hello: " << "\n";
            outs() << M.getName() << "\n";
            auto main = M.getFunction("main");
            if (!main) {
                return false;
            } else {
                outs() << "get main" << "\n";
            }
            return false;
        }
    };
}

char MyModulePass::ID = 0;

static RegisterPass<MyModulePass> X("mymodule", "My Module Pass");

static void registerPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
    PM.add(new MyModulePass());
}

static RegisterStandardPasses
        RegisterTheSpindlePass(PassManagerBuilder::EP_EarlyAsPossible, registerPass);
