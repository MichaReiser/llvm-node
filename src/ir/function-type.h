//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_FUNCTION_TYPE_H
#define LLVM_NODE_FUNCTION_TYPE_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include "type.h"
#include "../util/from-value-mixin.h"

class FunctionTypeWrapper: public TypeWrapper, FromValueMixin<FunctionTypeWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object Create(llvm::FunctionType* functionType);
    static bool isInstance(Napi::Value value);
    using FromValueMixin<FunctionTypeWrapper>::FromValue;
    llvm::FunctionType* getFunctionType();

private:
    explicit FunctionTypeWrapper(llvm::FunctionType* functionType):
            TypeWrapper { functionType }
    {}

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value get(const Napi::CallbackInfo& info);
    static Napi::Value isValidReturnType(const Napi::CallbackInfo& info);
    static Napi::Value isValidArgumentType(const Napi::CallbackInfo& info);

    // instance
    Napi::Value getReturnType(const Napi::CallbackInfo& info);
    Napi::Value isVarArg(const Napi::CallbackInfo& info);
    static Napi::Value getParamType(const Napi::CallbackInfo& info);
    Napi::Value getNumParams(const Napi::CallbackInfo& info);
    static Napi::Value getParams(const Napi::CallbackInfo& info);

    static Napi::FunctionReference& functionTypeTemplate();

};

#endif //LLVM_NODE_FUNCTION_TYPE_H
