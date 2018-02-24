//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_STRUCT_H
#define LLVM_NODE_CONSTANT_STRUCT_H

#include <nan.h>
#include <llvm/IR/Constants.h>
#include "constant.h"

class ConstantStructWrapper: public ConstantWrapper, public FromValueMixin<ConstantStructWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ConstantStruct* constantStruct);
    using FromValueMixin<ConstantStructWrapper>::FromValue;
    llvm::ConstantStruct* getConstantStruct();

private:
    explicit ConstantStructWrapper(llvm::ConstantStruct* constantStruct) : ConstantWrapper { constantStruct }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& constantStructTemplate();

    static NAN_METHOD(New);
    static NAN_METHOD(get);
};

#endif //LLVM_NODE_CONSTANT_STRUCT_H
