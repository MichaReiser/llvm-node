//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_INT_H
#define LLVM_NODE_CONSTANT_INT_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Constants.h>
#include "constant.h"

class ConstantIntWrapper: public ConstantWrapper, public FromValueMixin<ConstantIntWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::ConstantInt* constantInt);
    using FromValueMixin<ConstantIntWrapper>::FromValue;
    llvm::ConstantInt* getConstantInt();

private:
    explicit ConstantIntWrapper(llvm::ConstantInt* constant)
            : ConstantWrapper { constant }
    {}

    static Napi::FunctionReference& constantIntTemplate();

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value get(const Napi::CallbackInfo& info);
    static Napi::Value getTrue(const Napi::CallbackInfo& info);
    static Napi::Value getFalse(const Napi::CallbackInfo& info);

    // instance
    Napi::Value getValueApf(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_CONSTANT_INT_H
