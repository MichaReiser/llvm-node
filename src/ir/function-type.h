//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_FUNCTION_TYPE_H
#define LLVM_NODE_FUNCTION_TYPE_H

#include <nan.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>

#include "type.h"
#include "../util/from-value-mixin.h"

class FunctionTypeWrapper: public TypeWrapper, FromValueMixin<FunctionTypeWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> Create(llvm::FunctionType* functionType);
    static bool isInstance(v8::Local<v8::Value> value);
    using FromValueMixin<FunctionTypeWrapper>::FromValue;
    llvm::FunctionType* getFunctionType();

private:
    explicit FunctionTypeWrapper(llvm::FunctionType* functionType):
            TypeWrapper { functionType }
    {}

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);
    static NAN_METHOD(isValidReturnType);
    static NAN_METHOD(isValidArgumentType);

    // instance
    static NAN_GETTER(getReturnType);
    static NAN_GETTER(isVarArg);
    static NAN_METHOD(getParamType);
    static NAN_GETTER(getNumParams);
    static NAN_METHOD(getParams);

    static Nan::Persistent<v8::FunctionTemplate>& functionTypeTemplate();

};

#endif //LLVM_NODE_FUNCTION_TYPE_H
