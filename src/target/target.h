//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_TARGET_H
#define LLVM_NODE_TARGET_H

#include <napi.h>
#include <uv.h>
#include "target-machine.h"

Napi::Object InitTarget(Napi::Env env, Napi::Object exports) {
    TargetMachineWrapper::Init(env, target, module);
}

#endif //LLVM_NODE_TARGET_H
