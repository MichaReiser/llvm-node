//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_CONSTANTFPWRAPPER_H
#define LLVM_NODE_CONSTANTFPWRAPPER_H

#include <llvm/IR/Constants.h>
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(Constant, Constant, constant, Value, protected)
    // static
    static NAN_METHOD(New);
    static NAN_METHOD(getNullValue);
    static NAN_METHOD(getAllOnesValue);

    // instance
    static NAN_METHOD(isNullValue);
    static NAN_METHOD(isOneValue);
    static NAN_METHOD(isAllOnesValue);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_CONSTANTFPWRAPPER_H
