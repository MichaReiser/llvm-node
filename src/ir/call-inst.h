//
// Created by Micha Reiser on 13.03.17.
//

#ifndef LLVM_NODE_CALL_INST_H
#define LLVM_NODE_CALL_INST_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Instructions.h>
#include "value.h"

class CallInstWrapper: public ValueWrapper, public FromValueMixin<CallInstWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::CallInst* callInst);
    static bool isInstance(Napi::Value value);
    using FromValueMixin<CallInstWrapper>::FromValue;
    llvm::CallInst* getCallInst();

private:
    static Napi::Value New(const Napi::CallbackInfo& info);

    Napi::Value getCallingConv(const Napi::CallbackInfo& info);
    void setCallingConv(const Napi::CallbackInfo& info, const Napi::Value& value);
    static Napi::Value addDereferenceableAttr(const Napi::CallbackInfo& info);
    static Napi::Value hasRetAttr(const Napi::CallbackInfo& info);
    static Napi::Value paramHasAttr(const Napi::CallbackInfo& info);
    static Napi::Value getNumArgOperands(const Napi::CallbackInfo& info);

    explicit CallInstWrapper(llvm::CallInst* value) : ValueWrapper { value } {}

    static Napi::FunctionReference& callInstTemplate();
};

#endif //LLVM_NODE_CALL_INST_H
