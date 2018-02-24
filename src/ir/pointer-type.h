//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_POINTER_TYPE_H
#define LLVM_NODE_POINTER_TYPE_H

#include <nan.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include "llvm-context.h"
#include "../util/from-value-mixin.h"
#include "type.h"

class PointerTypeWrapper: public TypeWrapper, public FromValueMixin<PointerTypeWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::PointerType *type);
    using FromValueMixin<PointerTypeWrapper>::FromValue;

    llvm::PointerType* getPointerType();
    static bool isInstance(v8::Local<v8::Value> value);

protected:
    llvm::Type* type;
    explicit PointerTypeWrapper(llvm::PointerType* type) : TypeWrapper { type } {
    }

    static Nan::Persistent<v8::FunctionTemplate>& pointerTypeTemplate();

private:
    // Static Methods
    static NAN_METHOD(New);
    static NAN_METHOD(get);
    static NAN_GETTER(getElementType);
};

#endif //LLVM_NODE_POINTER_TYPE_H
