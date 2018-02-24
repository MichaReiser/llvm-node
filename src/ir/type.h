//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_TYPE_H
#define LLVM_NODE_TYPE_H

#include <napi.h>
#include <llvm/IR/Type.h>
#include "llvm-context.h"

class TypeWrapper: public Napi::ObjectWrap<TypeWrapper> {
public:
    static Napi::FunctionReference constructor;
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(Napi::Env env, llvm::Type *type);
    static bool isInstanceOfType(Napi::Value value);

    TypeWrapper(const Napi::CallbackInfo& info);
    llvm::Type* getType();

protected:
    llvm::Type* type;
private:
    static Napi::Object getIntNTy(const Napi::CallbackInfo& info);

    Napi::Object getPointerTo(const Napi::CallbackInfo& info);
    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getTypeID(const Napi::CallbackInfo& info);
    Napi::Value getPrimitiveSizeInBits(const Napi::CallbackInfo& info);
    Napi::Value toString(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_TYPE_H
