//
// Created by Micha Reiser on 13.03.17.
//

#ifndef LLVM_NODE_CALL_INST_H
#define LLVM_NODE_CALL_INST_H

#include <llvm/IR/Instructions.h>
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(CallInst, CallInst, callInst, Value, private)
    static NAN_METHOD(New);

    static NAN_GETTER(getCallingConv);
    static NAN_SETTER(setCallingConv);
    static NAN_METHOD(addDereferenceableAttr);
    static NAN_METHOD(hasRetAttr);
    static NAN_METHOD(paramHasAttr);
    static NAN_METHOD(getNumArgOperands);

public:
    static bool isInstance(v8::Local<v8::Value> value);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_CALL_INST_H
