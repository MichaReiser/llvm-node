//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-data-array.h"
#include "llvm-context.h"
#include "../util/array.h"
#include "../util/string.h"

Napi::Object ConstantDataArrayWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto constantDataArray = Napi::GetFunction(Napi::New(env, constantDataArrayTemplate()));
    (target).Set(Napi::String::New(env, "ConstantDataArray"), constantDataArray);
}

Napi::Object ConstantDataArrayWrapper::of(llvm::ConstantDataArray* constantDataArray) {
    Napi::EscapableHandleScope escapeScope {};
    auto constuctorFunction = Napi::GetFunction(Napi::New(env, constantDataArrayTemplate()));
    Napi::Value args[1] = { Napi::External::New(env, constantDataArray) };
    auto instance = Napi::NewInstance(constuctorFunction, 1, args);

    return escapeScope.Escape(instance);
}

llvm::ConstantDataArray* ConstantDataArrayWrapper::getConstantDataArray() {
    return static_cast<llvm::ConstantDataArray*>(getValue());
}

Napi::FunctionReference& ConstantDataArrayWrapper::constantDataArrayTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Napi::Function::New(env, ConstantDataArrayWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "ConstantDataArray"));

        localTemplate->Inherit(Napi::New(env, constantTemplate()));

        Napi::SetMethod(localTemplate, "get", ConstantDataArrayWrapper::get);
        Napi::SetMethod(localTemplate, "getString", ConstantDataArrayWrapper::getString);


        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

Napi::Value ConstantDataArrayWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "The constructor of ConstantDataArray needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "The constructor of ConstantDataArray needs to be called width: value: External").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = static_cast<llvm::ConstantDataArray*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new ConstantDataArrayWrapper { value };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value ConstantDataArrayWrapper::getString(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !LLVMContextWrapper::isInstance(info[0]) || !info[1].IsString()) {
        Napi::TypeError::New(env, "getString needs to be called with: context: LLVMContext, value: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto& llvmContext = LLVMContextWrapper::FromValue(info[0])->getContext();
    auto initializer = ToString(info[1]);

    auto* constantArray = llvm::ConstantDataArray::getString(llvmContext, initializer);
    return ConstantWrapper::of(constantArray);
}

Napi::Value ConstantDataArrayWrapper::get(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !LLVMContextWrapper::isInstance(info[0]) ||
            !(info[1].IsUint32Array() || info[1].IsFloat32Array() || info[1].IsFloat64Array())) {
        Napi::TypeError::New(env, "get needs to be called with: context: LLVMContext, arrayRef: UInt32Array | Float32Array | Float64Array").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    llvm::Constant* constant;

    if (info[1].IsUint32Array()) {
        auto ints = toVector<uint32_t , v8::Uint32Array>(info[1]);
        constant = llvm::ConstantDataArray::get(context, ints);
    } else if (info[1].IsFloat32Array()) {
        auto floatArray = info[1].As<v8::Float32Array>();
        std::vector<float> floats (floatArray->Length());

        for (uint32_t i = 0; i < floatArray->Length(); ++i) {
            auto nativeValue = (floatArray).Get(i);
            floats[i] = static_cast<float>(nativeValue.As<Napi::Number>().DoubleValue());
        }
        constant = llvm::ConstantDataArray::get(context, floats);
    } else {
        auto doubles = toVector<double, v8::Float64Array>(info[1]);
        constant = llvm::ConstantDataArray::get(context, doubles);
    }

    return ConstantWrapper::of(constant);
}