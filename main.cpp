#include "llvm/Pass.h"

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
class CallGraphPass : public ModulePass {
public:
  static char ID;

  CallGraphPass() : ModulePass(ID) {}

  bool runOnModule(Module &M) override {
    outs() << "hello\n";
    // Function *main = M.getFunction("main");
    // if (!main) {
    //   return false;
    // }
    // outs() << main->getName();
    return true;
  }
};
} // namespace

char CallGraphPass::ID = 0;

// static RegisterPass<CallGraphPass> X("mypass", "CallGraphPass", false, false);
static llvm::RegisterStandardPasses Y(
    llvm::PassManagerBuilder::EP_EarlyAsPossible,
    [](const llvm::PassManagerBuilder &Builder,
       llvm::legacy::PassManagerBase &PM) { PM.add(new CallGraphPass()); });
