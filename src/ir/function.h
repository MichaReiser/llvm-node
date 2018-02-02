//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_FUNCTION_H
#define LLVM_NODE_FUNCTION_H

#include <nan.h>
#include <llvm/IR/Function.h>
#include "constant.h"
#include "../util/from-value-mixin.h"

class FunctionWrapper: public ConstantWrapper, public FromValueMixin<FunctionWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::Function* function);

    llvm::Function* getFunction();

    static bool isInstance(v8::Local<v8::Value> value);
    using FromValueMixin<FunctionWrapper>::FromValue;

private:
    explicit FunctionWrapper(llvm::Function* function);

    // static
    static NAN_METHOD(New);
    static NAN_METHOD(Create);

    // instance
    static NAN_METHOD(addAttribute);
    static NAN_METHOD(addBasicBlock);
    static NAN_METHOD(addDereferenceableAttr);
    static NAN_METHOD(addDereferenceableOrNullAttr);
    static NAN_METHOD(addFnAttr);
    static NAN_METHOD(getArguments);
    static NAN_METHOD(getEntryBlock);
    static NAN_METHOD(getBasicBlocks);
    static NAN_GETTER(getCallingConv);
    static NAN_SETTER(setCallingConv);
    static NAN_GETTER(getVisibility);
    static NAN_SETTER(setVisibility);
    static NAN_METHOD(viewCFG);

    static Nan::Persistent<v8::FunctionTemplate>& functionTemplate();
};


#endif //LLVM_NODE_FUNCTION_H
