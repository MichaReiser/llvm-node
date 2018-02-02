//
// Created by Micha Reiser on 13.03.17.
//

#ifndef LLVM_NODE_STRUCT_TYPE_H
#define LLVM_NODE_STRUCT_TYPE_H

#include <nan.h>
#include <llvm/IR/DerivedTypes.h>
#include "type.h"

class StructTypeWrapper: public TypeWrapper, public FromValueMixin<StructTypeWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::StructType *type);
    static bool isInstance(v8::Local<v8::Value> value);
    using FromValueMixin<StructTypeWrapper>::FromValue;
    llvm::StructType* getStructType();

private:
    explicit StructTypeWrapper(llvm::StructType *type) : TypeWrapper { type } {
    }

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);
    static NAN_METHOD(create);

    // instance
    static NAN_GETTER(getName);
    static NAN_SETTER(setName);
    static NAN_GETTER(getNumElements);
    static NAN_METHOD(getElementType);

    static NAN_METHOD(setBody);

    static Nan::Persistent<v8::FunctionTemplate>& structTypeTemplate();
};

#endif //LLVM_NODE_STRUCT_TYPE_H
