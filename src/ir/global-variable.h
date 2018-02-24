//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_GLOBAL_VARIABLE_H
#define LLVM_NODE_GLOBAL_VARIABLE_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/GlobalVariable.h>
#include "constant.h"
#include "../util/from-value-mixin.h"

class GlobalVariableWrapper: public ConstantWrapper, public FromValueMixin<GlobalVariableWrapper> {
public:

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::GlobalVariable* variable);
    using FromValueMixin<GlobalVariableWrapper>::FromValue;
    llvm::GlobalVariable* getGlobalVariable();


private:
    explicit GlobalVariableWrapper(llvm::GlobalVariable* variable) : ConstantWrapper { variable } {}

    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value NewFromExternal(const Napi::CallbackInfo& info);
    static Napi::Value NewFromArguments(const Napi::CallbackInfo& info);

    static Napi::Value hasGlobalUnnamedAddr(const Napi::CallbackInfo& info);
    static Napi::Value setUnnamedAddr(const Napi::CallbackInfo& info);

    Napi::Value getConstant(const Napi::CallbackInfo& info);
    void setConstant(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value getInitializer(const Napi::CallbackInfo& info);
    void setInitializer(const Napi::CallbackInfo& info, const Napi::Value& value);
    static Napi::FunctionReference& globalVariableTemplate();

};

#endif //LLVM_NODE_GLOBAL_VARIABLE_H
