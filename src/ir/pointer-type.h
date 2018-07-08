//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_POINTER_TYPE_H
#define LLVM_NODE_POINTER_TYPE_H

#include <nan.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include "type.h"

class PointerTypeWrapper: public Napi::ObjectWrap<PointerTypeWrapper> {
public:
    static void Init(Napi::Env env, Napi::Object& exports);
    static Napi::Object of(Napi::Env env, llvm::PointerType *type);
    static bool isInstanceOfType(const Napi::Value& value);

    explicit PointerTypeWrapper(const Napi::CallbackInfo& info);

    llvm::PointerType* getPointerType();

private:
    static Napi::FunctionReference constructor;
    llvm::PointerType* pointerType;

    static Napi::Value get(const Napi::CallbackInfo& info);
    Napi::Value getElementType(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_POINTER_TYPE_H
