//
// Created by Micha Reiser on 24.05.17.
//

#ifndef LLVM_NODE_UNDEF_VALUE_H
#define LLVM_NODE_UNDEF_VALUE_H

#include <llvm/IR/Constants.h>
#include "constant.h"
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(UndefValue, UndefValue, undefValue, Constant, private)
    // static
    static NAN_METHOD(New);
    static NAN_METHOD(get);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_UNDEF_VALUE_H
