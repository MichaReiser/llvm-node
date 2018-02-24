//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_LLVM_CONTEXT_H
#define LLVM_NODE_LLVM_CONTEXT_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/LLVMContext.h>
#include "../util/from-value-mixin.h"

class LLVMContextWrapper : public Napi::ObjectWrap<LLVMContextWrapper>, public FromValueMixin<LLVMContextWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    static bool isInstance(Napi::Value value);
    llvm::LLVMContext& getContext();

    static Napi::Object of(llvm::LLVMContext &llvmContext);


private:
    static Napi::FunctionReference functionTemplate;
    llvm::LLVMContext context;
    LLVMContextWrapper() = default;

    static Napi::Value New(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_LLVM_CONTEXT_H
