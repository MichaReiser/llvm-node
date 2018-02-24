//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_FUNCTION_H
#define LLVM_NODE_FUNCTION_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Function.h>
#include "constant.h"
#include "../util/from-value-mixin.h"

class FunctionWrapper: public ConstantWrapper, public FromValueMixin<FunctionWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::Function* function);

    llvm::Function* getFunction();

    static bool isInstance(Napi::Value value);
    using FromValueMixin<FunctionWrapper>::FromValue;

private:
    explicit FunctionWrapper(llvm::Function* function);

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value Create(const Napi::CallbackInfo& info);

    // instance
    static Napi::Value addAttribute(const Napi::CallbackInfo& info);
    static Napi::Value addBasicBlock(const Napi::CallbackInfo& info);
    static Napi::Value addDereferenceableAttr(const Napi::CallbackInfo& info);
    static Napi::Value addDereferenceableOrNullAttr(const Napi::CallbackInfo& info);
    static Napi::Value addFnAttr(const Napi::CallbackInfo& info);
    static Napi::Value getArguments(const Napi::CallbackInfo& info);
    static Napi::Value getEntryBlock(const Napi::CallbackInfo& info);
    static Napi::Value getBasicBlocks(const Napi::CallbackInfo& info);
    Napi::Value getCallingConv(const Napi::CallbackInfo& info);
    void setCallingConv(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value getVisibility(const Napi::CallbackInfo& info);
    void setVisibility(const Napi::CallbackInfo& info, const Napi::Value& value);
    static Napi::Value viewCFG(const Napi::CallbackInfo& info);

    static Napi::FunctionReference& functionTemplate();
};


#endif //LLVM_NODE_FUNCTION_H
