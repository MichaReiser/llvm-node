//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_DATA_ARRAY_H
#define LLVM_NODE_CONSTANT_DATA_ARRAY_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Constants.h>
#include "constant.h"

class ConstantDataArrayWrapper: public ConstantWrapper, public FromValueMixin<ConstantDataArrayWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::ConstantDataArray* constantDataArray);
    using FromValueMixin<ConstantDataArrayWrapper>::FromValue;
    llvm::ConstantDataArray* getConstantDataArray();

private:
    explicit ConstantDataArrayWrapper(llvm::ConstantDataArray* constant)
            : ConstantWrapper { constant }
    {}

    static Napi::FunctionReference& constantDataArrayTemplate();

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value getString(const Napi::CallbackInfo& info);
    static Napi::Value get(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_CONSTANT_DATA_ARRAY_H
