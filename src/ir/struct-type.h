//
// Created by Micha Reiser on 13.03.17.
//

#ifndef LLVM_NODE_STRUCT_TYPE_H
#define LLVM_NODE_STRUCT_TYPE_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/DerivedTypes.h>
#include "type.h"

class StructTypeWrapper: public TypeWrapper, public FromValueMixin<StructTypeWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::StructType *type);
    static bool isInstance(Napi::Value value);
    using FromValueMixin<StructTypeWrapper>::FromValue;
    llvm::StructType* getStructType();

private:
    explicit StructTypeWrapper(llvm::StructType *type) : TypeWrapper { type } {
    }

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value get(const Napi::CallbackInfo& info);
    static Napi::Value create(const Napi::CallbackInfo& info);

    // instance
    Napi::Value getName(const Napi::CallbackInfo& info);
    void setName(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value getNumElements(const Napi::CallbackInfo& info);
    static Napi::Value getElementType(const Napi::CallbackInfo& info);

    static Napi::Value setBody(const Napi::CallbackInfo& info);

    static Napi::FunctionReference& structTypeTemplate();
};

#endif //LLVM_NODE_STRUCT_TYPE_H
