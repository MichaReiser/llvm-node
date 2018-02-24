//
// Created by Micha Reiser on 11.03.17.
//

#ifndef LLVM_NODE_ARRAY_TYPE_H
#define LLVM_NODE_ARRAY_TYPE_H\


#include <nan.h>
#include <llvm/IR/DerivedTypes.h>
#include "type.h"

class ArrayTypeWrapper : public TypeWrapper, public FromValueMixin<ArrayTypeWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ArrayType* type);
    static bool isInstance(v8::Local<v8::Value> value);
    using FromValueMixin<ArrayTypeWrapper>::FromValue;
    llvm::ArrayType* getArrayType();
private:
    explicit ArrayTypeWrapper(llvm::ArrayType* type) : TypeWrapper { type } {
    }

    static NAN_METHOD(New);
    static NAN_METHOD(get);
    static NAN_GETTER(getNumElements);
    static NAN_GETTER(getElementType);

    static v8::Persistent<v8::FunctionTemplate>& arrayTypeTemplate();
};

#endif //LLVM_NODE_ARRAY_TYPE_H
