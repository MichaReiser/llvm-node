//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_IR_BUILDER_H
#define LLVM_NODE_IR_BUILDER_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/IRBuilder.h>
#include <nan_callbacks_12_inl.h>
#include "../util/from-value-mixin.h"
#include "value.h"
#include "../util/string.h"

class IRBuilderWrapper : public Napi::ObjectWrap<IRBuilderWrapper>, public FromValueMixin<IRBuilderWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    llvm::IRBuilder<>& getIRBuilder();

private:
    llvm::IRBuilder<> irBuilder;

    explicit IRBuilderWrapper(llvm::IRBuilder<> irBuilder) : irBuilder { irBuilder } {
    }

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);

    // instance
    typedef llvm::Value* (llvm::IRBuilder<>::*ConvertOperationFn)(llvm::Value*, llvm::Type*, const llvm::Twine&);
    template<ConvertOperationFn method>
    static Napi::Value ConvertOperation(const Napi::CallbackInfo& info);

    static Napi::Value CreateAlloca(const Napi::CallbackInfo& info);
    static Napi::Value CreateAlignedLoad(const Napi::CallbackInfo& info);
    static Napi::Value CreateAlignedStore(const Napi::CallbackInfo& info);
    static Napi::Value CreateBr(const Napi::CallbackInfo& info);
    static Napi::Value CreateCall(const Napi::CallbackInfo& info);
    static Napi::Value CreateCondBr(const Napi::CallbackInfo& info);
    static Napi::Value CreateExtractValue(const Napi::CallbackInfo& info);
    static Napi::Value CreateFNeg(const Napi::CallbackInfo& info);
    static Napi::Value CreateGlobalStringPtr(const Napi::CallbackInfo& info);
    static Napi::Value CreateGlobalString(const Napi::CallbackInfo& info);
    static Napi::Value CreateInBoundsGEP(const Napi::CallbackInfo& info);
    static Napi::Value CreateInBoundsGEPWithoutType(const Napi::CallbackInfo& info);
    static Napi::Value CreateInBoundsGEPWithType(const Napi::CallbackInfo& info);
    static Napi::Value CreateInsertValue(const Napi::CallbackInfo& info);
    static Napi::Value CreateIntCast(const Napi::CallbackInfo& info);
    static Napi::Value CreateLoad(const Napi::CallbackInfo& info);
    static Napi::Value CreateNeg(const Napi::CallbackInfo& info);
    static Napi::Value CreateNot(const Napi::CallbackInfo& info);
    static Napi::Value CreatePHI(const Napi::CallbackInfo& info);
    static Napi::Value CreateRet(const Napi::CallbackInfo& info);
    static Napi::Value CreateRetVoid(const Napi::CallbackInfo& info);
    static Napi::Value CreateSelect(const Napi::CallbackInfo& info);
    static Napi::Value CreateStore(const Napi::CallbackInfo& info);
    static Napi::Value GetInsertBlock(const Napi::CallbackInfo& info);
    static Napi::Value SetInsertionPoint(const Napi::CallbackInfo& info);

    static inline Napi::FunctionReference& irBuilderConstructor() {
        static Napi::FunctionReference constructor {};
        return constructor;
    }
};

#endif //LLVM_NODE_IR_BUILDER_H
