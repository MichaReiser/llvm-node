//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_ARGUMENT_H
#define LLVM_NODE_ARGUMENT_H

#include <llvm/IR/Argument.h>
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(Argument, Argument, argument, Value, private)
    static NAN_METHOD(New);
    static NAN_GETTER(getArgNo);
    static NAN_GETTER(getParent);
    static NAN_METHOD(addAttr);
    static NAN_METHOD(hasAttribute);
    static NAN_METHOD(addDereferenceableAttr);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_ARGUMENT_H
