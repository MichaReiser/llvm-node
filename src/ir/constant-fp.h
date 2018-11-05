//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_FP_WRAPPER_H
#define LLVM_NODE_CONSTANT_FP_WRAPPER_H

#include <nan.h>
#include <llvm/IR/Constants.h>
#include "constant.h"
#include "../util/from-value-mixin.h"

class ConstantFPWrapper: public ConstantWrapper, public FromValueMixin<ConstantFPWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ConstantFP* constantFP);
    using FromValueMixin<ConstantFPWrapper>::FromValue;
    llvm::ConstantFP* getConstantFP();

private:
    explicit ConstantFPWrapper(llvm::ConstantFP* constant)
            : ConstantWrapper { constant }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& constantFpTemplate();

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);
    static NAN_METHOD(getZeroValueForNegation);
    static NAN_METHOD(getNegativeZero);
    static NAN_METHOD(getNaN);
    static NAN_METHOD(getInfinity);

    // instance
    static NAN_GETTER(getValueAPF);
};

#endif //LLVM_NODE_CONSTANT_FP_WRAPPER_H
