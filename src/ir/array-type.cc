//
// Created by Micha Reiser on 11.03.17.
//

#include "array-type.h"

Napi::Object ArrayTypeWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto arrayType = Napi::GetFunction(Napi::New(env, arrayTypeTemplate()));
    (target).Set(Napi::String::New(env, "ArrayType"), arrayType);
}

Napi::Object ArrayTypeWrapper::of(llvm::ArrayType* arrayType) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, arrayTypeTemplate()));
    Napi::Value args[1] = { Napi::New<v8::External> (arrayType) };

    auto instance = Napi::NewInstance(constructorFunction, 1, args);

    Napi::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

bool ArrayTypeWrapper::isInstance(Napi::Value value) {
    Napi::Env env = value.Env();
    return Napi::New(env, arrayTypeTemplate())->HasInstance(value);
}

llvm::ArrayType* ArrayTypeWrapper::getArrayType() {
    return static_cast<llvm::ArrayType*>(getType());
}

Napi::Value ArrayTypeWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Constructor needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "ArrayType constructor needs to be called with: arrayType: external").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* arrayType = static_cast<llvm::ArrayType*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new ArrayTypeWrapper { arrayType };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value ArrayTypeWrapper::get(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !TypeWrapper::isInstance(info[0]) || !info[1].IsUint32()) {
        Napi::TypeError::New(env, "get needs to be called with: elementType: Type, numElements: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* elementType = TypeWrapper::FromValue(info[0])->getType();
    auto numElements = info[1].As<Napi::Number>().Uint32Value();

    auto* arrayType = llvm::ArrayType::get(elementType, numElements);

    return ArrayTypeWrapper::of(arrayType);
}

Napi::Value ArrayTypeWrapper::getNumElements(const Napi::CallbackInfo& info) {
    auto* arrayType = ArrayTypeWrapper::FromValue(info.Holder())->getArrayType();
    uint32_t numElements = arrayType->getNumElements();

    return Napi::New(env, numElements);
}

Napi::Value ArrayTypeWrapper::getElementType(const Napi::CallbackInfo& info) {
    auto* arrayType = ArrayTypeWrapper::FromValue(info.Holder())->getArrayType();
    auto* elementType = arrayType->getElementType();

    return TypeWrapper::of(elementType);
}

v8::Persistent<Napi::FunctionReference>& ArrayTypeWrapper::arrayTypeTemplate() {
    static Napi::FunctionReference tmpl {};

    if (tmpl.IsEmpty()) {
        auto arrayTypeWrapperTemplate = Napi::Function::New(env, ArrayTypeWrapper::New);
        arrayTypeWrapperTemplate->SetClassName(Napi::String::New(env, "ArrayType"));

        arrayTypeWrapperTemplate->Inherit(Napi::New(env, typeTemplate()));

        Napi::SetMethod(arrayTypeWrapperTemplate, "get", ArrayTypeWrapper::get);
        Napi::SetAccessor(arrayTypeWrapperTemplate->InstanceTemplate(), Napi::String::New(env, "numElements"), ArrayTypeWrapper::getNumElements);
        Napi::SetAccessor(arrayTypeWrapperTemplate->InstanceTemplate(), Napi::String::New(env, "elementType"), ArrayTypeWrapper::getElementType);

        tmpl.Reset(arrayTypeWrapperTemplate);
    }

    return tmpl;
}
