//
// Created by Micha Reiser on 04.04.17.
//

#ifndef LLVM_NODE_CONSTANT_ARRAY_H
#define LLVM_NODE_CONSTANT_ARRAY_H

#include <nan.h>
#include "../util/from-value-mixin.h"
#include "constant.h"

class ConstantArrayWrapper: public ConstantWrapper, public FromValueMixin<ConstantArrayWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ConstantArray* constantArray);
    using FromValueMixin<ConstantArrayWrapper>::FromValue;

private:
    explicit ConstantArrayWrapper(llvm::ConstantArray* array)
            : ConstantWrapper { array }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& constantArrayTemplate();

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);
};

#endif //LLVM_NODE_CONSTANT_ARRAY_H
