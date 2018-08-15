//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_PHINODEWRAPPER_H
#define LLVM_NODE_PHINODEWRAPPER_H

#include <llvm/IR/Instructions.h>
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(PHINode, PhiNode, phiNode, Value, private)
    static NAN_METHOD(New);
    static NAN_METHOD(addIncoming);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_PHINODEWRAPPER_H
