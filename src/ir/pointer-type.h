//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_POINTER_TYPE_H
#define LLVM_NODE_POINTER_TYPE_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include "llvm-context.h"
#include "../util/from-value-mixin.h"
#include "type.h"

class PointerTypeWrapper: public TypeWrapper, public FromValueMixin<PointerTypeWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::PointerType *type);
    using FromValueMixin<PointerTypeWrapper>::FromValue;

    llvm::PointerType* getPointerType();
    static bool isInstance(Napi::Value value);

protected:
    llvm::Type* type;
    explicit PointerTypeWrapper(llvm::PointerType* type) : TypeWrapper { type } {
    }

    static Napi::FunctionReference& pointerTypeTemplate();

private:
    // Static Methods
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value get(const Napi::CallbackInfo& info);
    Napi::Value getElementType(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_POINTER_TYPE_H
