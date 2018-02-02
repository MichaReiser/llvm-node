//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_DATA_ARRAY_H
#define LLVM_NODE_CONSTANT_DATA_ARRAY_H

#include <nan.h>
#include <llvm/IR/Constants.h>
#include "constant.h"

class ConstantDataArrayWrapper: public ConstantWrapper, public FromValueMixin<ConstantDataArrayWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ConstantDataArray* constantDataArray);
    using FromValueMixin<ConstantDataArrayWrapper>::FromValue;
    llvm::ConstantDataArray* getConstantDataArray();

private:
    explicit ConstantDataArrayWrapper(llvm::ConstantDataArray* constant)
            : ConstantWrapper { constant }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& constantDataArrayTemplate();

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(getString);
    static NAN_METHOD(get);
};

#endif //LLVM_NODE_CONSTANT_DATA_ARRAY_H
