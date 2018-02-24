//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_BASICBLOCKWRAPPER_H
#define LLVM_NODE_BASICBLOCKWRAPPER_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/BasicBlock.h>
#include "value.h"

class BasicBlockWrapper: public ValueWrapper, public FromValueMixin<BasicBlockWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::BasicBlock* basicBlock);
    static bool isInstance(Napi::Value value);
    using FromValueMixin<BasicBlockWrapper>::FromValue;
    inline llvm::BasicBlock* getBasicBlock() {
        return static_cast<llvm::BasicBlock*>(getValue());
    }

private:
    explicit BasicBlockWrapper(llvm::BasicBlock* basicBlock)
            : ValueWrapper { basicBlock }
    {}

    // static methods
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value Create(const Napi::CallbackInfo& info);

    // instance methods
    static Napi::Value eraseFromParent(const Napi::CallbackInfo& info);
    Napi::Value empty(const Napi::CallbackInfo& info);
    Napi::Value getParent(const Napi::CallbackInfo& info);
    static Napi::Value getTerminator(const Napi::CallbackInfo& info);
    Napi::Value getContext(const Napi::CallbackInfo& info);

    static Napi::FunctionReference& basicBlockTemplate();
};


#endif //LLVM_NODE_BASICBLOCKWRAPPER_H
