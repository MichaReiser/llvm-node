//
// Created by Micha Reiser on 01.03.17.
//

#include <llvm/IR/InstrTypes.h>
#include "function.h"
#include "llvm-context.h"
#include "basic-block.h"
#include "../util/string.h"

Napi::Object BasicBlockWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto basicBlock = Napi::GetFunction(Napi::New(env, basicBlockTemplate()));
    (target).Set(Napi::String::New(env, "BasicBlock"), basicBlock);
}

Napi::Object BasicBlockWrapper::of(llvm::BasicBlock *basicBlock) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, basicBlockTemplate()));
    Napi::Value argv[1] = { Napi::External::New(env, basicBlock) };
    auto instance = Napi::NewInstance(constructorFunction, 1, argv);

    Napi::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

Napi::Value BasicBlockWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Class Constructor BasicBlock cannot be invoked without new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "BasicBlock constructor requires: basicBlock: External").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* basicBlock = static_cast<llvm::BasicBlock*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new BasicBlockWrapper { basicBlock };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value BasicBlockWrapper::Create(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0])
        || (info.Length() > 1 && !info[1].IsString())
        || (info.Length() > 2 && !FunctionWrapper::isInstance(info[2]))
        || (info.Length() > 3 && !BasicBlockWrapper::isInstance(info[3]))
        || info.Length() > 4) {
        Napi::TypeError::New(env, "BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?").ThrowAsJavaScriptException();
        return env.Null();
    }

    llvm::LLVMContext& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    std::string name {};
    llvm::Function* parent = nullptr;
    llvm::BasicBlock* insertBefore = nullptr;

    if (info.Length() > 1) {
        name = ToString(info[1].To<Napi::String>());
    }

    if (info.Length() > 2) {
        parent = FunctionWrapper::FromValue(info[2])->getFunction();
    }

    if (info.Length() > 3) {
        insertBefore = BasicBlockWrapper::FromValue(info[3])->getBasicBlock();
    }

    auto* basicBlock = llvm::BasicBlock::Create(context, name, parent, insertBefore);
    return BasicBlockWrapper::of(basicBlock);
}

Napi::Value BasicBlockWrapper::empty(const Napi::CallbackInfo& info) {
    auto* basicBlock = BasicBlockWrapper::FromValue(info.Holder())->getBasicBlock();
    return Napi::New(env, basicBlock->empty());
}

Napi::Value BasicBlockWrapper::eraseFromParent(const Napi::CallbackInfo& info) {
    auto* basicBlock = BasicBlockWrapper::FromValue(info.Holder())->getBasicBlock();
    basicBlock->eraseFromParent();
}

Napi::Value BasicBlockWrapper::getParent(const Napi::CallbackInfo& info) {
    auto* wrapper = BasicBlockWrapper::FromValue(info.Holder());
    auto* function = wrapper->getBasicBlock()->getParent();

    if (function) {
        auto functionObject = FunctionWrapper::of(function);
        return return functionObject;
    }
}

Napi::Value BasicBlockWrapper::getContext(const Napi::CallbackInfo& info) {
    auto* wrapper = BasicBlockWrapper::FromValue(info.Holder());
    auto& context = wrapper->getBasicBlock()->getContext();

    return LLVMContextWrapper::of(context);
}


Napi::Value BasicBlockWrapper::getTerminator(const Napi::CallbackInfo& info) {
    auto* basicBlock = BasicBlockWrapper::FromValue(info.Holder())->getBasicBlock();
    auto* terminator = basicBlock->getTerminator();

    if (terminator) {
        return return ValueWrapper::of(terminator);
    }

    return env.Undefined();
}

Napi::FunctionReference& BasicBlockWrapper::basicBlockTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Napi::Function::New(env, BasicBlockWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "BasicBlock"));

        localTemplate->Inherit(Napi::New(env, ValueWrapper::valueTemplate()));

        Napi::SetMethod(localTemplate, "create", BasicBlockWrapper::Create);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "empty"), BasicBlockWrapper::empty);
        Napi::SetPrototypeMethod(localTemplate, "eraseFromParent", BasicBlockWrapper::eraseFromParent);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "parent"), BasicBlockWrapper::getParent);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "context"), BasicBlockWrapper::getContext);
        Napi::SetPrototypeMethod(localTemplate, "getTerminator", BasicBlockWrapper::getTerminator);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

bool BasicBlockWrapper::isInstance(Napi::Value value) {
    Napi::Env env = value.Env();
    return Napi::New(env, basicBlockTemplate())->HasInstance(value);
}
