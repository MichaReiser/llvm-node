//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_BASICBLOCKWRAPPER_H
#define LLVM_NODE_BASICBLOCKWRAPPER_H

#include <nan.h>
#include <llvm/IR/BasicBlock.h>
#include "value.h"

class BasicBlockWrapper: public ValueWrapper, public FromValueMixin<BasicBlockWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::BasicBlock* basicBlock);
    static bool isInstance(v8::Local<v8::Value> value);
    using FromValueMixin<BasicBlockWrapper>::FromValue;
    inline llvm::BasicBlock* getBasicBlock() {
        return static_cast<llvm::BasicBlock*>(getValue());
    }

private:
    explicit BasicBlockWrapper(llvm::BasicBlock* basicBlock)
            : ValueWrapper { basicBlock }
    {}

    // static methods
    static NAN_METHOD(New);
    static NAN_METHOD(Create);

    // instance methods
    static NAN_METHOD(eraseFromParent);
    static NAN_GETTER(empty);
    static NAN_GETTER(getParent);
    static NAN_METHOD(getTerminator);
    static NAN_GETTER(getContext);

    static Nan::Persistent<v8::FunctionTemplate>& basicBlockTemplate();
};


#endif //LLVM_NODE_BASICBLOCKWRAPPER_H
