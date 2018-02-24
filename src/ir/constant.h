//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_CONSTANTFPWRAPPER_H
#define LLVM_NODE_CONSTANTFPWRAPPER_H

#include <napi.h>
#include <llvm/IR/Constants.h>
#include "value.h"

class ConstantWrapper : public Napi::ObjectWrap<ConstantWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    static Napi::Object of(Napi::Env env, llvm::Constant *constant);

    static Napi::FunctionReference constructor;

    ConstantWrapper(const Napi::CallbackInfo &info);

    llvm::Constant *getConstant();

private:
    static Napi::Value getNullValue(const Napi::CallbackInfo &info);

    static Napi::Value getAllOnesValue(const Napi::CallbackInfo &info);

    // instance
    Napi::Value isNullValue(const Napi::CallbackInfo &info);

    Napi::Value isOneValue(const Napi::CallbackInfo &info);

    Napi::Value isAllOnesValue(const Napi::CallbackInfo &info);
};

#endif //LLVM_NODE_CONSTANTFPWRAPPER_H
