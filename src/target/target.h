//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_TARGET_H
#define LLVM_NODE_TARGET_H

#include <nan.h>
#include "target-machine.h"

NAN_MODULE_INIT(InitTarget) {
    TargetMachineWrapper::Init(target);
}

#endif //LLVM_NODE_TARGET_H
