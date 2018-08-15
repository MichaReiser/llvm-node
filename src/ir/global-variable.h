//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_GLOBAL_VARIABLE_H
#define LLVM_NODE_GLOBAL_VARIABLE_H

#include <llvm/IR/GlobalVariable.h>
#include "constant.h"
#include "value-wrapper-template.h"

VALUE_WRAPPER_TEMPLATE_BEGIN(GlobalVariable, GlobalVariable, globalVariable, Constant, private)
    static NAN_METHOD(New);
    static NAN_METHOD(NewFromExternal);
    static NAN_METHOD(NewFromArguments);

    static NAN_METHOD(hasGlobalUnnamedAddr);
    static NAN_METHOD(setUnnamedAddr);

    static NAN_GETTER(getConstant);
    static NAN_SETTER(setConstant);

    static NAN_GETTER(getInitializer);
    static NAN_SETTER(setInitializer);
VALUE_WRAPPER_TEMPLATE_END

#endif //LLVM_NODE_GLOBAL_VARIABLE_H
