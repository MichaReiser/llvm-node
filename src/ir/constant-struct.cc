//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-struct.h"
#include "struct-type.h"

Napi::Object ConstantStructWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto constantStruct = Napi::GetFunction(Napi::New(env, constantStructTemplate()));

    (target).Set(Napi::String::New(env, "ConstantStruct"), constantStruct);
}

Napi::Object ConstantStructWrapper::of(llvm::ConstantStruct* constantStruct) {
    auto constructor = Napi::GetFunction(Napi::New(env, constantStructTemplate()));
    Napi::Value args[1] = { Napi::New<v8::External> (constantStruct) };

    auto instance = Napi::NewInstance(constructor, 1, args);

    Napi::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::ConstantStruct* ConstantStructWrapper::getConstantStruct() {
    return static_cast<llvm::ConstantStruct*>(getValue());
}

Napi::FunctionReference& ConstantStructWrapper::constantStructTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        Napi::FunctionReference localTemplate = Napi::Function::New(env, ConstantStructWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "ConstantStruct"));

        localTemplate->Inherit(Napi::New(env, constantTemplate()));

        Napi::SetMethod(localTemplate, "get", ConstantStructWrapper::get);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

Napi::Value ConstantStructWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "ConstantStruct constructor needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsExternal()) {
        Napi::TypeError::New(env, "ConstantStruct constructor needs to be called with: constantStruct: External").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* constantStruct = static_cast<llvm::ConstantStruct*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new ConstantStructWrapper { constantStruct };
    wrapper->Wrap(info.This());

    return info.This();
}


Napi::Value ConstantStructWrapper::get(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !StructTypeWrapper::isInstance(info[0]) || !info[1].IsArray()) {
        Napi::TypeError::New(env, "get needs to be called with: structType: StructType, values: Constant[]").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* structType = StructTypeWrapper::FromValue(info[0])->getStructType();
    auto array = info[1].As<Napi::Array>();
    std::vector<llvm::Constant*> constants { array->Length() };

    for (uint32_t i = 0; i < array->Length(); ++i) {
        auto value = (array).Get(i);
        if (!ConstantWrapper::isInstance(value)) {
            Napi::TypeError::New(env, "get expected array of Constant values").ThrowAsJavaScriptException();
            return env.Null();
        }

        constants[i] = ConstantWrapper::FromValue(value)->getConstant();
    }

    auto constantStruct = llvm::ConstantStruct::get(structType, constants);

    return ConstantWrapper::of(constantStruct);
}