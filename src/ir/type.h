//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_TYPE_H
#define LLVM_NODE_TYPE_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Type.h>
#include "llvm-context.h"
#include "../util/from-value-mixin.h"

class TypeWrapper : public Napi::ObjectWrap<TypeWrapper>, public FromValueMixin<TypeWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::Type *type);
    using FromValueMixin::FromValue;

    llvm::Type* getType();

    static bool isInstance(Napi::Value value);

protected:
    llvm::Type* type;

    explicit TypeWrapper(llvm::Type* type) : Napi::ObjectWrap {}, type { type } {
        assert(type && "No type pointer passed");
    }

    static Napi::FunctionReference& typeTemplate();

    // Static Methods
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value getPointerTo(const Napi::CallbackInfo& info);
    static Napi::Value getIntNTy(const Napi::CallbackInfo& info);

private:
    // Instance Methods
    static Napi::Value equals(const Napi::CallbackInfo& info);
    Napi::Value getTypeID(const Napi::CallbackInfo& info);
    static Napi::Value getPrimitiveSizeInBits(const Napi::CallbackInfo& info);
    static Napi::Value toString(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_TYPE_H
