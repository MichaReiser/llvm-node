#ifndef LLVM_NODE_CONSTANT_AGGREGATE_ZERO_H
#define LLVM_NODE_CONSTANT_AGGREGATE_ZERO_H

#include <nan.h>
#include <llvm/IR/Constants.h>
#include "constant.h"

class ConstantAggregateZeroWrapper: public ConstantWrapper, public FromValueMixin<ConstantAggregateZeroWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ConstantAggregateZero* constantAggregateZero);
    using FromValueMixin<ConstantAggregateZeroWrapper>::FromValue;
    llvm::ConstantAggregateZero* getConstantAggregateZero();

private:
    explicit ConstantAggregateZeroWrapper(llvm::ConstantAggregateZero* constantAggregateZero) : ConstantWrapper { constantAggregateZero }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& constantAggregateZeroTemplate();

    static NAN_METHOD(New);
    static NAN_METHOD(get);
};

#endif //LLVM_NODE_CONSTANT_AGGREGATE_ZERO_H
