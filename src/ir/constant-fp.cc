//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-fp.h"
#include "llvm-context.h"
#include "type.h"

Napi::Object ConstantFPWrapper::Init(Napi::Env env, Napi::Object exports) {
        auto constantFp = Napi::GetFunction(Napi::New(env, constantFpTemplate()));
        (target).Set(Napi::String::New(env, "ConstantFP"), constantFp);
}

Napi::Value ConstantFPWrapper::New(const Napi::CallbackInfo& info) {
        if (!info.IsConstructCall()) {
            Napi::TypeError::New(env, "Class Constructor ConstantFP cannot be invoked without new").ThrowAsJavaScriptException();
            return env.Null();
        }

        if (info.Length() != 1 || !info[0].IsExternal()) {
            Napi::TypeError::New(env, "ConstantFP constructor needs to be called with: constantFP: external").ThrowAsJavaScriptException();
            return env.Null();
        }

        auto* constantFp = static_cast<llvm::ConstantFP*>(*info[0].As<Napi::External>()->Value());
        auto* wrapper = new ConstantFPWrapper { constantFp };
        wrapper->Wrap(info.This());

        return info.This();
}

Napi::Value ConstantFPWrapper::get(const Napi::CallbackInfo& info) {
        if (info.Length() != 2 || !LLVMContextWrapper::isInstance(info[0]) || !info[1].IsNumber()) {
            Napi::TypeError::New(env, "get needs to be called with: context: LLVMContext, value: number").ThrowAsJavaScriptException();
            return env.Null();
        }

        auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
        double number = info[1].As<Napi::Number>().DoubleValue();

        auto* constant = llvm::ConstantFP::get(context, llvm::APFloat { number } );

        return ConstantFPWrapper::of(constant);
}

Napi::Value ConstantFPWrapper::getNaN(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "getNaN needs to be called with: type: Type").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    llvm::Constant* nan = llvm::ConstantFP::getNaN(type);

    return ConstantWrapper::of(nan);
}

Napi::Value ConstantFPWrapper::getValueAPF(const Napi::CallbackInfo& info) {
        auto* wrapper = ConstantFPWrapper::FromValue(info.Holder());
        auto value = wrapper->getConstantFP()->getValueAPF();

        return Napi::New(env, value.convertToDouble());
}

llvm::ConstantFP *ConstantFPWrapper::getConstantFP() {
    return static_cast<llvm::ConstantFP*>(getValue());
}

Napi::Object ConstantFPWrapper::of(llvm::ConstantFP *constantFP) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, constantFpTemplate()));
    Napi::Value args[1] = { Napi::External::New(env, constantFP) };
    auto instance = Napi::NewInstance(constructorFunction, 1, args);

    Napi::EscapableHandleScope escapeScpoe {};
    return escapeScpoe.Escape(instance);
}

Napi::FunctionReference& ConstantFPWrapper::constantFpTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Napi::Function::New(env, ConstantFPWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "ConstantFP"));

        localTemplate->Inherit(Napi::New(env, constantTemplate()));

        Napi::SetMethod(localTemplate, "get", ConstantFPWrapper::get);
        Napi::SetMethod(localTemplate, "getNaN", ConstantFPWrapper::getNaN);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "value"), ConstantFPWrapper::getValueAPF);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}
