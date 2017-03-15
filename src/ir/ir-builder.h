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

class IRBuilderWrapper: public Nan::ObjectWrap, public FromValueMixin<IRBuilderWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    llvm::IRBuilder<>& getIRBuilder();

private:
    llvm::IRBuilder<> irBuilder;

    IRBuilderWrapper(llvm::IRBuilder<> irBuilder) : irBuilder { irBuilder } {
    }

    // static
    static NAN_METHOD(New);

    // instance
    typedef llvm::Value* (llvm::IRBuilder<>::*ConvertOperationFn)(llvm::Value*, llvm::Type*, const llvm::Twine&);
    template<ConvertOperationFn method>
    static NAN_METHOD(ConvertOperation);

    static NAN_METHOD(CreateAlloca);
    static NAN_METHOD(CreateAlignedLoad);
    static NAN_METHOD(CreateAlignedStore);
    static NAN_METHOD(CreateBr);
    static NAN_METHOD(CreateCall);
    static NAN_METHOD(CreateCondBr);
    static NAN_METHOD(CreateInBoundsGEP);
    static Nan::NAN_METHOD_RETURN_TYPE CreateInBoundsGEPWithoutType(Nan::NAN_METHOD_ARGS_TYPE info);
    static Nan::NAN_METHOD_RETURN_TYPE CreateInBoundsGEPWithType(Nan::NAN_METHOD_ARGS_TYPE info);
    static NAN_METHOD(CreateIntCast);
    static NAN_METHOD(CreateLoad);
    static NAN_METHOD(CreatePHI);
    static NAN_METHOD(CreateRet);
    static NAN_METHOD(CreateRetVoid);
    static NAN_METHOD(CreateStore);
    static NAN_METHOD(GetInsertBlock);
    static NAN_METHOD(SetInsertionPoint);

    static inline Nan::Persistent<v8::Function>& irBuilderConstructor() {
        static Nan::Persistent<v8::Function> constructor {};
        return constructor;
    }
};

#endif //LLVM_NODE_IR_BUILDER_H
