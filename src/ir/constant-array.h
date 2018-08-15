//
// Created by Micha Reiser on 04.04.17.
//

#ifndef LLVM_NODE_CONSTANT_ARRAY_H
#define LLVM_NODE_CONSTANT_ARRAY_H

#include <nan.h>
#include "constant.h"
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(ConstantArray, ConstantArray, constantArray, Constant, private)
    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_CONSTANT_ARRAY_H
