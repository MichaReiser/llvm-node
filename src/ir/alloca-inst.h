//
// Created by Micha Reiser on 03.03.17.
//

#ifndef LLVM_NODE_ALLOCAINST_H
#define LLVM_NODE_ALLOCAINST_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Instructions.h>
#include "value.h"

class AllocaInstWrapper: public ValueWrapper, public FromValueMixin<AllocaInstWrapper> {
public:
    using FromValueMixin<AllocaInstWrapper>::FromValue;

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::AllocaInst* allocaInst);
    llvm::AllocaInst* getAllocaInst();

private:
    explicit AllocaInstWrapper(llvm::AllocaInst* allocaInst): ValueWrapper { allocaInst } {
    }

    static Napi::Value New(const Napi::CallbackInfo& info);
    Napi::Value getAllocatedType(const Napi::CallbackInfo& info);
    void setAllocatedType(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value getAlignment(const Napi::CallbackInfo& info);
    void setAlignment(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value getArraySize(const Napi::CallbackInfo& info);
    static Napi::FunctionReference& allocaInstTemplate();
};


#endif //LLVM_NODE_ALLOCAINST_H
