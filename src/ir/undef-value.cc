//
// Created by Micha Reiser on 24.05.17.
//

#include "undef-value.h"
#include "llvm-context.h"
#include "type.h"

Napi::Object UndefValueWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto undefValue = Napi::GetFunction(Napi::New(env, undefValueTemplate()));
    (target).Set(Napi::String::New(env, "UndefValue"), undefValue);
}

Napi::Value UndefValueWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Class Constructor UndefValueWrapper cannot be invoked without new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "UndefValueWrapper constructor needs to be called with: undefValueWrapper: external").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* undefValue = static_cast<llvm::UndefValue*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new UndefValueWrapper { undefValue };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value UndefValueWrapper::get(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "get needs to be called with: type: Type").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto* undefValue = llvm::UndefValue::get(type);
    return UndefValueWrapper::of(undefValue);
}

llvm::UndefValue* UndefValueWrapper::getUndefValue() {
    return static_cast<llvm::UndefValue*>(getValue());
}

Napi::Object UndefValueWrapper::of(llvm::UndefValue* undefValue) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, undefValueTemplate()));
    Napi::Value args[1] = { Napi::External::New(env, undefValue) };
    auto instance = Napi::NewInstance(constructorFunction, 1, args);

    Napi::EscapableHandleScope escapeScpoe {};
    return escapeScpoe.Escape(instance);
}

Napi::FunctionReference& UndefValueWrapper::undefValueTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Napi::Function::New(env, UndefValueWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "UndefValue"));

        localTemplate->Inherit(Napi::New(env, constantTemplate()));

        Napi::SetMethod(localTemplate, "get", UndefValueWrapper::get);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}