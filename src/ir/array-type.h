//
// Created by Micha Reiser on 11.03.17.
//

#ifndef LLVM_NODE_ARRAY_TYPE_H
#define LLVM_NODE_ARRAY_TYPE_H\


#include <napi.h>
#include <uv.h>
#include <llvm/IR/DerivedTypes.h>
#include "type.h"

class ArrayTypeWrapper : public TypeWrapper, public FromValueMixin<ArrayTypeWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::ArrayType* type);
    static bool isInstance(Napi::Value value);
    using FromValueMixin<ArrayTypeWrapper>::FromValue;
    llvm::ArrayType* getArrayType();
private:
    explicit ArrayTypeWrapper(llvm::ArrayType* type) : TypeWrapper { type } {
    }

    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value get(const Napi::CallbackInfo& info);
    Napi::Value getNumElements(const Napi::CallbackInfo& info);
    Napi::Value getElementType(const Napi::CallbackInfo& info);

    static v8::Persistent<Napi::FunctionReference>& arrayTypeTemplate();
};

#endif //LLVM_NODE_ARRAY_TYPE_H
