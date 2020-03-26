
#include <llvm/IR/Argument.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/TypeBuilder.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Pass.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

using namespace llvm;
using namespace std;

Constant *CreateWords(Module *m, const string &str);

namespace {
    class DynamicFunctionPass : public FunctionPass {
    public:
        static char ID;
        Function *recordFunction = nullptr;
        Function *printFunction = nullptr;

        DynamicFunctionPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
            Module *m = F.getParent();
            if (!recordFunction) {
                FunctionType *recordType = TypeBuilder<void(char *, char *), false>::get(F.getContext());
                recordFunction = cast<Function>(m->getOrInsertFunction(
                        "__record__",
                        recordType,

                        AttributeList().addAttribute(m->getContext(), 0U, Attribute::NoAlias)));
            }
            if (!printFunction) {
                FunctionType *printType = TypeBuilder<void(), false>::get(F.getContext());
                printFunction = cast<Function>(m->getOrInsertFunction(
                        "__print__",
                        printType,
                        AttributeList().addAttribute(m->getContext(), 0U, Attribute::NoAlias)));
            }

            Instruction *I = nullptr;

            for (auto &bb : F) {
                for (auto &ii : bb) {
                    I = &ii;
                    if (auto *inst = dyn_cast_or_null<CallInst>(I)) {
                        Function *called = inst->getCalledFunction();
                        if (called) {
                            CallInst::Create(recordFunction, ArrayRef<Value *>{
                                    CreateWords(m, F.getName()),
                                    CreateWords(m, called->getName())
                            }, "", inst);
                            if (called->getName() == "exit") {
                                outs() << "call exit function\n";
                                CallInst::Create(printFunction, ArrayRef<Value *>{}, "", I);
                            }
                        }
                    }
                }
            }

            if (F.getName() == "main") {
                CallInst::Create(printFunction, ArrayRef<Value *>{}, "", I);
            }

            return false;
        }
    };
} // namespace

char DynamicFunctionPass::ID = 0;

static RegisterPass<DynamicFunctionPass> X("DynamicFunctionPass", "My DynamicFunctionPass Pass");

static void registerPass(const PassManagerBuilder &, legacy::PassManagerBase &PM) {
    PM.add(new DynamicFunctionPass());
}

static RegisterStandardPasses RegisterTheSpindlePass(PassManagerBuilder::EP_EarlyAsPossible, registerPass);

// Create words to parameter
Constant *CreateWords(Module *m, const string &str) {
    // look up the word in global value table
    string mystr = "%%" + str;
    GlobalValue *my_name = m->getNamedValue(mystr);
    if (my_name) {
        auto *my_global = cast<GlobalVariable>(my_name);
        ConstantInt *const_int = ConstantInt::get(m->getContext(), APInt(32, 0));
        std::vector<Constant *> const_ptr_indices;
        const_ptr_indices.push_back(const_int);
        const_ptr_indices.push_back(const_int);

        // add nullptr paramater to adapt the function (by jiang zuming)
        Constant *const_ptr = ConstantExpr::getGetElementPtr(nullptr, my_global, const_ptr_indices);
        return const_ptr;
    }

    // if not find it, then create a new word in global value table
    ArrayType *ArrayTy = ArrayType::get(IntegerType::get(m->getContext(), 8), str.length() + 1);
    auto *global_name = new GlobalVariable(*m, ArrayTy, true, GlobalValue::PrivateLinkage, nullptr, mystr);
    global_name->setAlignment(1);

    // Constant Definitions
    Constant *const_array = ConstantDataArray::getString(m->getContext(), str, true);
    ConstantInt *const_int = ConstantInt::get(m->getContext(), APInt(32, 0));
    std::vector<Constant *> const_ptr_indices;
    const_ptr_indices.push_back(const_int);
    const_ptr_indices.push_back(const_int);

    Constant *const_ptr = ConstantExpr::getGetElementPtr(nullptr, global_name, const_ptr_indices);
    // Global Variable Definitions
    global_name->setInitializer(const_array);
    return const_ptr;
}