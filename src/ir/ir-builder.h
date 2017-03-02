//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_IR_BUILDER_H
#define LLVM_NODE_IR_BUILDER_H

#include <nan.h>
#include <llvm/IR/IRBuilder.h>
#include <nan_callbacks_12_inl.h>
#include "../util/from-value-mixin.h"
#include "value.h"
#include "../util/string.h"

class IRBuilderWrapper: public Nan::ObjectWrap, FromValueMixin<IRBuilderWrapper> {
public:
    static NAN_MODULE_INIT(Init);
private:
    llvm::IRBuilder<> irBuilder;

    IRBuilderWrapper(llvm::LLVMContext& context) : irBuilder { context } {
    }

    // static
    static NAN_METHOD(New);

    // instance
    typedef llvm::Value* (llvm::IRBuilder<>::*BinaryOpFn)(llvm::Value*, llvm::Value*, const llvm::Twine&, llvm::MDNode *FPMathTag);
    template<BinaryOpFn method>
    static NAN_METHOD(BinaryOperation);
    static NAN_METHOD(CreateBr);
    static NAN_METHOD(CreateCall);
    static NAN_METHOD(CreateCondBr);
    static NAN_METHOD(CreatePHI);
    static NAN_METHOD(CreateRet);
    static NAN_METHOD(CreateRetVoid);
    static NAN_METHOD(GetInsertBlock);
    static NAN_METHOD(SetInsertionPoint);

    static inline Nan::Persistent<v8::Function>& irBuilderConstructor() {
        static Nan::Persistent<v8::Function> constructor {};
        return constructor;
    }
};

#endif //LLVM_NODE_IR_BUILDER_H
