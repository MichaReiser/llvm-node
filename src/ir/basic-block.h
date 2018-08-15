//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_BASICBLOCKWRAPPER_H
#define LLVM_NODE_BASICBLOCKWRAPPER_H

#include <llvm/IR/BasicBlock.h>
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(BasicBlock, BasicBlock, basicBlock, Value, private)
    // static methods
    static NAN_METHOD(New);
    static NAN_METHOD(Create);

    // instance methods
    static NAN_METHOD(eraseFromParent);
    static NAN_GETTER(empty);
    static NAN_GETTER(getParent);
    static NAN_METHOD(getTerminator);
    static NAN_GETTER(getContext);

public:
    static bool isInstance(v8::Local<v8::Value> value);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_BASICBLOCKWRAPPER_H
