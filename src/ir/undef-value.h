//
// Created by Micha Reiser on 24.05.17.
//

#ifndef LLVM_NODE_UNDEF_VALUE_H
#define LLVM_NODE_UNDEF_VALUE_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Constants.h>
#include "constant.h"

class UndefValueWrapper: public ConstantWrapper, public FromValueMixin<UndefValueWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::UndefValue* undefValue);
    using FromValueMixin<UndefValueWrapper>::FromValue;
    llvm::UndefValue* getUndefValue();

private:
    explicit UndefValueWrapper(llvm::UndefValue* constant)
            : ConstantWrapper { constant }
    {}

    static Napi::FunctionReference& undefValueTemplate();

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value get(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_UNDEF_VALUE_H
