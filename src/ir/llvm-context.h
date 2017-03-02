//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_LLVM_CONTEXT_H
#define LLVM_NODE_LLVM_CONTEXT_H

#include <nan.h>
#include <llvm/IR/LLVMContext.h>
#include "../util/from-value-mixin.h"

class LLVMContextWrapper: public Nan::ObjectWrap, public FromValueMixin<LLVMContextWrapper> {
public:
    static NAN_MODULE_INIT(Init);

    static bool isInstance(v8::Local<v8::Value> value);
    llvm::LLVMContext& get();

private:
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    llvm::LLVMContext context;
    LLVMContextWrapper() {}

    static NAN_METHOD(New);
};

#endif //LLVM_NODE_LLVM_CONTEXT_H
