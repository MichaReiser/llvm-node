//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-int.h"
#include "llvm-context.h"

Napi::Object ConstantIntWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto constantInt = Napi::GetFunction(Napi::New(env, constantIntTemplate()));
    (target).Set(Napi::String::New(env, "ConstantInt"), constantInt);
}

Napi::Value ConstantIntWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Class Constructor ConstantInt cannot be invoked without new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "ConstantInt constructor needs to be called with: constantInt: external").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* constantInt = static_cast<llvm::ConstantInt*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new ConstantIntWrapper { constantInt };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value ConstantIntWrapper::get(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !LLVMContextWrapper::isInstance(info[0]) || !info[1].IsNumber() ||
            (info.Length() > 2 && !info[2].IsNumber() && !info[2].IsUndefined()) ||
            (info.Length() > 3 && !info[3].IsBoolean()) ||
            info.Length() > 4) {
        Napi::TypeError::New(env, "get needs to be called with: context: LLVMContext, value: number, numBits = 32, signed= true").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    int64_t number = Napi::To<int64_t >(info[1]);
    uint32_t numBits = 32;
    bool isSigned = true;

    if (info.Length() > 2 && !info[2].IsUndefined()) {
        numBits = info[2].As<Napi::Number>().Uint32Value();
    }

    if (info.Length() == 3) {
        isSigned = info[3].As<Napi::Boolean>().Value();
    }

    auto* constant = llvm::ConstantInt::get(context, llvm::APInt { numBits, static_cast<uint64_t>(number), isSigned } );

    return ConstantIntWrapper::of(constant);
}

Napi::Value ConstantIntWrapper::getTrue(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !LLVMContextWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "getTrue needs to be called with: context: LLVMContext").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    auto* constant = llvm::ConstantInt::getTrue(context);

    return ConstantIntWrapper::of(constant);
}

Napi::Value ConstantIntWrapper::getFalse(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !LLVMContextWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "getFalse needs to be called with: context: LLVMContext").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    auto* constant = llvm::ConstantInt::getFalse(context);

    return ConstantIntWrapper::of(constant);
}

Napi::Value ConstantIntWrapper::getValueApf(const Napi::CallbackInfo& info) {
    auto* wrapper = ConstantIntWrapper::FromValue(info.Holder());
    auto value = wrapper->getConstantInt()->getValue();

    return Napi::New(env, value.signedRoundToDouble());
}

llvm::ConstantInt *ConstantIntWrapper::getConstantInt() {
    return static_cast<llvm::ConstantInt*>(getValue());
}

Napi::Object ConstantIntWrapper::of(llvm::ConstantInt *constantInt) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, constantIntTemplate()));
    Napi::Value args[1] = { Napi::External::New(env, constantInt) };
    auto instance = Napi::NewInstance(constructorFunction, 1, args);

    Napi::EscapableHandleScope escapeScpoe {};
    return escapeScpoe.Escape(instance);
}

Napi::FunctionReference& ConstantIntWrapper::constantIntTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Napi::Function::New(env, ConstantIntWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "ConstantInt"));

        localTemplate->Inherit(Napi::New(env, constantTemplate()));

        Napi::SetMethod(localTemplate, "get", ConstantIntWrapper::get);
        Napi::SetMethod(localTemplate, "getFalse", ConstantIntWrapper::getFalse);
        Napi::SetMethod(localTemplate, "getTrue", ConstantIntWrapper::getTrue);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "value"), ConstantIntWrapper::getValueApf);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}