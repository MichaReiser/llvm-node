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
    static void Init(Napi::Env env, Napi::Object& exports);
    static Napi::Object of(Napi::Env env, llvm::Type *type);
    static bool isInstanceOfType(const Napi::Value& value);

    explicit TypeWrapper(const Napi::CallbackInfo& info);

    llvm::Type* getType();

private:
    static Napi::FunctionReference constructor;
    llvm::Type* type;

    static Napi::Value getIntNTy(const Napi::CallbackInfo& info);
    Napi::Value getPointerTo(const Napi::CallbackInfo& info);
    Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getTypeID(const Napi::CallbackInfo& info);
    Napi::Value getPrimitiveSizeInBits(const Napi::CallbackInfo& info);
    Napi::Value toString(const Napi::CallbackInfo& info);
    Napi::Value isIntegerTy(const Napi::CallbackInfo& info);

    typedef bool (llvm::Type::*isTy)() const;
    template<isTy method>
    Napi::Value isOfType(const Napi::CallbackInfo& info) {
        return Napi::Boolean::New(info.Env(), (type->*method)());
    }
};

#endif //LLVM_NODE_TYPE_H
