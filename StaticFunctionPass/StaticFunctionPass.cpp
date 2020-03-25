
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
    class StaticFunctionPass : public FunctionPass {
    public:
        static char ID;

        vector<Node *> nodes;

        Node *main = nullptr;

        StaticFunctionPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override;

        bool doFinalization(Module &module) override;
    };
} // namespace

char StaticFunctionPass::ID = 0;

bool StaticFunctionPass::runOnFunction(Function &F) {
    // outs() << "Hello: " << F.getName() << "\n";
    Node *parentNode = Node::createOrGetFromVector(nodes, F.getName());
    if (F.getName() == "main") {
        main = parentNode;
    }

    if (parentNode->isScanned()) {
        return false;
    }

    for (auto &iter : F) {
        for (auto &bb : iter) {
            Instruction *I = &bb;
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

bool StaticFunctionPass::doFinalization(Module &module) {
    main->print(0);
    return Pass::doFinalization(module);
}

static RegisterPass<StaticFunctionPass> X("StaticFunctionPass", "My StaticFunctionPass Pass");

static void registerPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
    PM.add(new StaticFunctionPass());
}

static RegisterStandardPasses
        RegisterTheSpindlePass(PassManagerBuilder::EP_EarlyAsPossible,
                               registerPass);
