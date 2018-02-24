//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_STRUCT_H
#define LLVM_NODE_CONSTANT_STRUCT_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Constants.h>
#include "constant.h"

class ConstantStructWrapper: public ConstantWrapper, public FromValueMixin<ConstantStructWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::ConstantStruct* constantStruct);
    using FromValueMixin<ConstantStructWrapper>::FromValue;
    llvm::ConstantStruct* getConstantStruct();

private:
    explicit ConstantStructWrapper(llvm::ConstantStruct* constantStruct) : ConstantWrapper { constantStruct }
    {}

    static Napi::FunctionReference& constantStructTemplate();

    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value get(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_CONSTANT_STRUCT_H
