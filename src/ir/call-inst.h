//
// Created by Micha Reiser on 13.03.17.
//

#ifndef LLVM_NODE_CALL_INST_H
#define LLVM_NODE_CALL_INST_H

#include <nan.h>
#include <llvm/IR/Instructions.h>
#include "value.h"

class CallInstWrapper: public ValueWrapper, public FromValueMixin<CallInstWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::CallInst* callInst);
    static bool isInstance(v8::Local<v8::Value> value);
    using FromValueMixin<CallInstWrapper>::FromValue;
    llvm::CallInst* getCallInst();

private:
    static NAN_METHOD(New);

    static NAN_GETTER(getCallingConv);
    static NAN_SETTER(setCallingConv);
    static NAN_METHOD(addDereferenceableAttr);
    static NAN_METHOD(hasRetAttr);
    static NAN_METHOD(paramHasAttr);
    static NAN_METHOD(getNumArgOperands);

    explicit CallInstWrapper(llvm::CallInst* value) : ValueWrapper { value } {}

    static Nan::Persistent<v8::FunctionTemplate>& callInstTemplate();
};

#endif //LLVM_NODE_CALL_INST_H
