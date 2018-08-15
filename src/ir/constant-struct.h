//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_STRUCT_H
#define LLVM_NODE_CONSTANT_STRUCT_H

#include <llvm/IR/Constants.h>
#include "constant.h"
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(ConstantStruct, ConstantStruct, constantStruct, Constant, private)
    static NAN_METHOD(New);
    static NAN_METHOD(get);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_CONSTANT_STRUCT_H
