//
// Created by Micha Reiser on 04.04.17.
//

#include "constant-array.h"
#include "array-type.h"

Napi::Object ConstantArrayWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto constantArray = Napi::GetFunction(Napi::New(env, constantArrayTemplate()));
    (target).Set(Napi::String::New(env, "ConstantArray"), constantArray);
}

Napi::Object ConstantArrayWrapper::of(llvm::ConstantArray* constantArray) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, constantArrayTemplate()));
    Napi::Value args[1] = { Napi::External::New(env, constantArray)};

    auto instance = Napi::NewInstance(constructorFunction, 1, args);

    Napi::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

Napi::FunctionReference& ConstantArrayWrapper::constantArrayTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        Napi::FunctionReference localTemplate = Napi::Function::New(env, ConstantArrayWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "ConstantArray"));

        localTemplate->Inherit(Napi::New(env, constantTemplate()));

        Napi::SetMethod(localTemplate, "get", ConstantArrayWrapper::get);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

Napi::Value ConstantArrayWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "The constructor of ConstantArray needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "The constructor of ConstantArray needs to be called width: value: External").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = static_cast<llvm::ConstantArray*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new ConstantArrayWrapper { value };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value ConstantArrayWrapper::get(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !ArrayTypeWrapper::isInstance(info[0]) || !info[1].IsArray()) {
        Napi::TypeError::New(env, "ConstantArray.get needs to be called with: type: ArrayType, values: constants").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* arrayType = ArrayTypeWrapper::FromValue(info[0])->getArrayType();
    auto jsArray = info[1].As<Napi::Array>();
    std::vector<llvm::Constant*> constants { jsArray->Length() };

    for (uint32_t i = 0; i < jsArray->Length(); ++i) {
        auto value = (jsArray).Get(i);
        if (!ConstantWrapper::isInstance(value)) {
            Napi::TypeError::New(env, "get expected array of Constant values").ThrowAsJavaScriptException();
            return env.Null();
        }

        constants[i] = ConstantWrapper::FromValue(value)->getConstant();
    }

    auto* result = llvm::ConstantArray::get(arrayType, constants);
    return ConstantWrapper::of(result);
}
