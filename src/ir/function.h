//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_FUNCTION_H
#define LLVM_NODE_FUNCTION_H

#include <llvm/IR/Function.h>
#include "constant.h"
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(Function, Function, function, Constant, private)
    // static
    static NAN_METHOD(New);
    static NAN_METHOD(Create);

    // instance
    static NAN_METHOD(addAttribute);
    static NAN_METHOD(addBasicBlock);
    static NAN_METHOD(addDereferenceableAttr);
    static NAN_METHOD(addDereferenceableOrNullAttr);
    static NAN_METHOD(addFnAttr);
    static NAN_METHOD(getArguments);
    static NAN_METHOD(getEntryBlock);
    static NAN_METHOD(getBasicBlocks);
    static NAN_GETTER(getCallingConv);
    static NAN_SETTER(setCallingConv);
    static NAN_GETTER(getVisibility);
    static NAN_SETTER(setVisibility);
    static NAN_METHOD(viewCFG);

public:
    static bool isInstance(v8::Local<v8::Value> value);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_FUNCTION_H
