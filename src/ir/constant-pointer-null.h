//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_NULL_POINTER_H
#define LLVM_NODE_CONSTANT_NULL_POINTER_H

#include <nan.h>
#include <llvm/IR/Constants.h>
#include "constant.h"
#include "../util/from-value-mixin.h"

class ConstantPointerNullWrapper: public ConstantWrapper, public FromValueMixin<ConstantPointerNullWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ConstantPointerNull* constantPointerNull);
    using FromValueMixin<ConstantPointerNullWrapper>::FromValue;
    llvm::ConstantPointerNull* getConstantPointerNull();

private:
    explicit ConstantPointerNullWrapper(llvm::ConstantPointerNull* constant)
            : ConstantWrapper { constant }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& constantPointerNullTemplate();

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);
};

#endif //LLVM_NODE_CONSTANT_NULL_POINTER_H
