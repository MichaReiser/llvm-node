//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_DATA_ARRAY_H
#define LLVM_NODE_CONSTANT_DATA_ARRAY_H

#include <llvm/IR/Constants.h>
#include "constant.h"
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(ConstantDataArray, ConstantDataArray, constantDataArray, Constant, private)
    // static
    static NAN_METHOD(New);
    static NAN_METHOD(getString);
    static NAN_METHOD(get);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_CONSTANT_DATA_ARRAY_H
