
#ifndef LLVM_NODE_EXECUTION_ENGINE_H
#define LLVM_NODE_EXECUTION_ENGINE_H

#include <nan.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include "../util/from-value-mixin.h"

class ExecutionEngineWrapper: public Nan::ObjectWrap, public FromValueMixin<ExecutionEngineWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ExecutionEngine *ptr);

private:
    llvm::ExecutionEngine* executionEngine;
    static Nan::Persistent<v8::ObjectTemplate> executionEngineTemplate;

    explicit ExecutionEngineWrapper(llvm::ExecutionEngine* executionEngine): executionEngine { executionEngine } {
        assert(executionEngine && "No execute engine passed");
    }

    static NAN_METHOD(addModule);
};

#endif //LLVM_NODE_EXECUTION_ENGINE_H
