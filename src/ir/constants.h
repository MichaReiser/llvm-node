//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_CONSTANTFPWRAPPER_H
#define LLVM_NODE_CONSTANTFPWRAPPER_H

#include <nan.h>
#include <llvm/IR/Constants.h>
#include "value.h"
#include "../util/from-value-mixin.h"

class ConstantWrapper: public ValueWrapper, public FromValueMixin<ConstantWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::Constant* constant);
    using FromValueMixin<ConstantWrapper>::FromValue;
    llvm::Constant* getConstant();

protected:
    static Nan::Persistent<v8::FunctionTemplate>& constantTemplate();
    ConstantWrapper(llvm::Constant *constant)
            : ValueWrapper {constant }
    {}

private:

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(getNullValue);
    static NAN_METHOD(getAllOnesValue);

    // instance
    static NAN_METHOD(isNullValue);
    static NAN_METHOD(isOneValue);
    static NAN_METHOD(isAllOnesValue);
};

class ConstantFPWrapper: public ConstantWrapper, public FromValueMixin<ConstantFPWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ConstantFP* constantFP);
    using FromValueMixin<ConstantFPWrapper>::FromValue;
    llvm::ConstantFP* getConstantFP();

private:
    ConstantFPWrapper(llvm::ConstantFP* constant)
            : ConstantWrapper { constant }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& constantFpTemplate();

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);

    // instance
    static NAN_GETTER(getValueAPF);
};


#endif //LLVM_NODE_CONSTANTFPWRAPPER_H
