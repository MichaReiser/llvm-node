//
// Created by Micha Reiser on 03.03.17.
//

#ifndef LLVM_NODE_ALLOCAINST_H
#define LLVM_NODE_ALLOCAINST_H

#include <llvm/IR/Instructions.h>
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(AllocaInst, AllocaInst, allocaInst, Value, private)
    static NAN_METHOD(New);
    static NAN_GETTER(getAllocatedType);
    static NAN_SETTER(setAllocatedType);
    static NAN_GETTER(getAlignment);
    static NAN_SETTER(setAlignment);
    static NAN_GETTER(getArraySize);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_ALLOCAINST_H
