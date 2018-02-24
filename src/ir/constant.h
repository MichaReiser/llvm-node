//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_CONSTANTFPWRAPPER_H
#define LLVM_NODE_CONSTANTFPWRAPPER_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Constants.h>
#include "value.h"
#include "../util/from-value-mixin.h"

class ConstantWrapper: public ValueWrapper, public FromValueMixin<ConstantWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::Constant* constant);
    using FromValueMixin<ConstantWrapper>::FromValue;
    llvm::Constant* getConstant();

protected:
    static Napi::FunctionReference& constantTemplate();
    explicit ConstantWrapper(llvm::Constant *constant)
            : ValueWrapper {constant }
    {}

private:

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value getNullValue(const Napi::CallbackInfo& info);
    static Napi::Value getAllOnesValue(const Napi::CallbackInfo& info);

    // instance
    static Napi::Value isNullValue(const Napi::CallbackInfo& info);
    static Napi::Value isOneValue(const Napi::CallbackInfo& info);
    static Napi::Value isAllOnesValue(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_CONSTANTFPWRAPPER_H
