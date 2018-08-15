//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_FP_WRAPPER_H
#define LLVM_NODE_CONSTANT_FP_WRAPPER_H

#include <llvm/IR/Constants.h>
#include "constant.h"
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(ConstantFP, ConstantFP, constantFp, Constant, private)
    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);
    static NAN_METHOD(getNaN);

    // instance
    static NAN_GETTER(getValueAPF);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_CONSTANT_FP_WRAPPER_H
