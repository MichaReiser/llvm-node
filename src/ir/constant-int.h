//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_INT_H
#define LLVM_NODE_CONSTANT_INT_H

#include <nan.h>
#include <llvm/IR/Constants.h>
#include "constant.h"

class ConstantIntWrapper: public ConstantWrapper, public FromValueMixin<ConstantIntWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ConstantInt* constantInt);
    using FromValueMixin<ConstantIntWrapper>::FromValue;
    llvm::ConstantInt* getConstantInt();

private:
    explicit ConstantIntWrapper(llvm::ConstantInt* constant)
            : ConstantWrapper { constant }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& constantIntTemplate();

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);
    static NAN_METHOD(getTrue);
    static NAN_METHOD(getFalse);
    static NAN_METHOD(toString);

    // instance
    static NAN_GETTER(getValueApf);
};

#endif //LLVM_NODE_CONSTANT_INT_H
