
#ifndef LLVM_NODE_EXECUTION_ENGINE_MODULE_H
#define LLVM_NODE_EXECUTION_ENGINE_MODULE_H

#include <nan.h>
#include "execution-engine.h"

NAN_MODULE_INIT(InitExecutionEngine) {
    ExecutionEngineWrapper::Init(target);
}

#endif //LLVM_NODE_EXECUTION_ENGINE_MODULE_H
