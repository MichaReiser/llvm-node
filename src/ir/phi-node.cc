//
// Created by Micha Reiser on 01.03.17.
//

#include "phi-node.h"
#include "basic-block.h"

llvm::PHINode *PhiNodeWrapper::getPhiNode() {
    return static_cast<llvm::PHINode*>(getValue());
}

Napi::Object PhiNodeWrapper::of(llvm::PHINode *phiNode) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, phiNodeTemplate()));
    Napi::Value args[1] = { Napi::External::New(env, phiNode) };
    auto instance = Napi::NewInstance(constructorFunction, 1, args);

    Napi::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

Napi::Object PhiNodeWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto functionTemplate = Napi::Function::New(env, PhiNodeWrapper::New);

    functionTemplate->SetClassName(Napi::String::New(env, "PHINode"));

    functionTemplate->Inherit(Napi::New(env, valueTemplate()));

    Napi::SetPrototypeMethod(functionTemplate, "addIncoming", PhiNodeWrapper::addIncoming);

    phiNodeTemplate().Reset(functionTemplate);
}

Napi::Value PhiNodeWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Class Constructor PhiNode cannot be invoked without new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "PhiNode constructor needs be called with external pointer to PHINode").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* phiNode = static_cast<llvm::PHINode*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new PhiNodeWrapper { phiNode };
    wrapper->Wrap(info.This());
    return info.This();
}

Napi::Value PhiNodeWrapper::addIncoming(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !ValueWrapper::isInstance(info[0]) || !BasicBlockWrapper::isInstance(info[1])) {
        Napi::TypeError::New(env, "addIncome needs to be called with: value: Value, basicBlock: BasicBlock").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* basicBlock = BasicBlockWrapper::FromValue(info[1])->getBasicBlock();

    PhiNodeWrapper::FromValue(info.Holder())->getPhiNode()->addIncoming(value, basicBlock);
}