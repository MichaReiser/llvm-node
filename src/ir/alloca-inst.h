//
// Created by Micha Reiser on 03.03.17.
//

#ifndef LLVM_NODE_ALLOCAINST_H
#define LLVM_NODE_ALLOCAINST_H

#include <nan.h>
#include <llvm/IR/Instructions.h>
#include "value.h"

class AllocaInstWrapper: public ValueWrapper, public FromValueMixin<AllocaInstWrapper> {
public:
    using FromValueMixin<AllocaInstWrapper>::FromValue;

    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::AllocaInst* allocaInst);
    llvm::AllocaInst* getAllocaInst();

private:
    explicit AllocaInstWrapper(llvm::AllocaInst* allocaInst): ValueWrapper { allocaInst } {
    }

    static NAN_METHOD(New);
    static NAN_GETTER(getAllocatedType);
    static NAN_SETTER(setAllocatedType);
    static NAN_GETTER(getAlignment);
    static NAN_SETTER(setAlignment);
    static NAN_GETTER(getArraySize);
    static Nan::Persistent<v8::FunctionTemplate>& allocaInstTemplate();
};


#endif //LLVM_NODE_ALLOCAINST_H
