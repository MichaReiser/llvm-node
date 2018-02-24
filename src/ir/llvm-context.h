//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_LLVM_CONTEXT_H
#define LLVM_NODE_LLVM_CONTEXT_H

#include <napi.h>
#include <llvm/IR/LLVMContext.h>

class LLVMContextWrapper: public Napi::ObjectWrap<LLVMContextWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static bool isInstanceOfType(const Napi::Value& value);
    LLVMContextWrapper(const Napi::CallbackInfo& info);
    llvm::LLVMContext& getContext();

private:
    static Napi::FunctionReference constructor;
    llvm::LLVMContext context;
};

#endif //LLVM_NODE_LLVM_CONTEXT_H
