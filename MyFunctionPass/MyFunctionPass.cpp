
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "Graph.hpp"

using namespace llvm;
using namespace std;

namespace {
class MyFunctionPass : public FunctionPass {
public:
  static char ID;

  vector<Node *> nodes;

  Node *main = nullptr;

  MyFunctionPass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override;

  bool doFinalization(Module &module) override;
};
} // namespace

char MyFunctionPass::ID = 0;

bool MyFunctionPass::runOnFunction(Function &F) {
  outs() << "Hello: " << F.getName() << "\n";
  Node *parentNode = Node::createOrGetFromVector(nodes, F.getName());
  if (F.getName() == "main") {
    main = parentNode;
  }

  if (parentNode->isScanned()) {
    return false;
  }

  for (Function::iterator iter = F.begin(); iter != F.end(); ++iter) {
    for (BasicBlock::iterator bb = iter->begin(); bb != iter->end(); ++bb) {
      Instruction *I = &*bb;
      if (auto *inst = dyn_cast_or_null<CallInst>(I)) {
        Function *called = inst->getCalledFunction();
        if (called) {
          Node *childNode =
              Node::createOrGetFromVector(nodes, called->getName());
          parentNode->addEdge(childNode);
        }
      }
    }
  }

  parentNode->setScan(true);

  return true;
}

bool MyFunctionPass::doFinalization(Module &module) {
  main->print(0);
  return Pass::doFinalization(module);
}

static RegisterPass<MyFunctionPass> X("myfuncpass", "My Function Pass");

static void registerPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new MyFunctionPass());
}
static RegisterStandardPasses
    RegisterTheSpindlePass(PassManagerBuilder::EP_EarlyAsPossible,
                           registerPass);
