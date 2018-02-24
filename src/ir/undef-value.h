//
// Created by Micha Reiser on 24.05.17.
//

#ifndef LLVM_NODE_UNDEF_VALUE_H
#define LLVM_NODE_UNDEF_VALUE_H

#include <nan.h>
#include <llvm/IR/Constants.h>
#include "constant.h"

class UndefValueWrapper: public ConstantWrapper, public FromValueMixin<UndefValueWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::UndefValue* undefValue);
    using FromValueMixin<UndefValueWrapper>::FromValue;
    llvm::UndefValue* getUndefValue();

private:
    explicit UndefValueWrapper(llvm::UndefValue* constant)
            : ConstantWrapper { constant }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& undefValueTemplate();

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);
};

#endif //LLVM_NODE_UNDEF_VALUE_H
