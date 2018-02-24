//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-pointer-null.h"
#include "pointer-type.h"

Napi::Object ConstantPointerNullWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto constantPointerNull = Napi::GetFunction(Napi::New(env, constantPointerNullTemplate()));
    (target).Set(Napi::String::New(env, "ConstantPointerNull"), constantPointerNull);
}

Napi::Value ConstantPointerNullWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Class Constructor ConstantPointerNull cannot be invoked without new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "ConstantPointerNull constructor needs to be called with: constantPointerNull: external").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* constantPointerNull = static_cast<llvm::ConstantPointerNull*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new ConstantPointerNullWrapper { constantPointerNull };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value ConstantPointerNullWrapper::get(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !PointerTypeWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "get needs to be called with: type: PointerType").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = PointerTypeWrapper::FromValue(info[0])->getPointerType();
    auto* constant = llvm::ConstantPointerNull::get(type);

    return ConstantPointerNullWrapper::of(constant);
}

llvm::ConstantPointerNull *ConstantPointerNullWrapper::getConstantPointerNull() {
    return static_cast<llvm::ConstantPointerNull*>(getValue());
}

Napi::Object ConstantPointerNullWrapper::of(llvm::ConstantPointerNull *constantPointerNull) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, constantPointerNullTemplate()));
    Napi::Value args[1] = { Napi::External::New(env, constantPointerNull) };
    auto instance = Napi::NewInstance(constructorFunction, 1, args);

    Napi::EscapableHandleScope escapeScpoe {};
    return escapeScpoe.Escape(instance);
}

Napi::FunctionReference& ConstantPointerNullWrapper::constantPointerNullTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Napi::Function::New(env, ConstantPointerNullWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "ConstantPointerNull"));

        localTemplate->Inherit(Napi::New(env, constantTemplate()));

        Napi::SetMethod(localTemplate, "get", ConstantPointerNullWrapper::get);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}