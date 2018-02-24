//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_FP_WRAPPER_H
#define LLVM_NODE_CONSTANT_FP_WRAPPER_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Constants.h>
#include "constant.h"
#include "../util/from-value-mixin.h"

class ConstantFPWrapper: public ConstantWrapper, public FromValueMixin<ConstantFPWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::ConstantFP* constantFP);
    using FromValueMixin<ConstantFPWrapper>::FromValue;
    llvm::ConstantFP* getConstantFP();

private:
    explicit ConstantFPWrapper(llvm::ConstantFP* constant)
            : ConstantWrapper { constant }
    {}

    static Napi::FunctionReference& constantFpTemplate();

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value get(const Napi::CallbackInfo& info);
    static Napi::Value getNaN(const Napi::CallbackInfo& info);

    // instance
    Napi::Value getValueAPF(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_CONSTANT_FP_WRAPPER_H
