//
// Created by Micha Reiser on 28.02.17.
//

#include "pointer-type.h"

Napi::Object PointerTypeWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto pointerType = Napi::GetFunction(Napi::New(env, pointerTypeTemplate()));
    (target).Set(Napi::String::New(env, "PointerType"), pointerType);
}

Napi::Value PointerTypeWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Constructor needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() < 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "Expected pointer type pointer").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = static_cast<llvm::PointerType*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new PointerTypeWrapper { type };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value PointerTypeWrapper::get(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !TypeWrapper::isInstance(info[0]) || !info[1].IsUint32()) {
        Napi::TypeError::New(env, "PointerTypeWrapper.get needs to be called with: elementType: Type, addressSpace: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    uint32_t as = info[1].As<Napi::Number>().Uint32Value();

    auto* pointerType = llvm::PointerType::get(type, as);

    return PointerTypeWrapper::of(pointerType);
}

Napi::Value PointerTypeWrapper::getElementType(const Napi::CallbackInfo& info) {
    auto* pointerType = PointerTypeWrapper::FromValue(info.Holder())->getPointerType();
    auto* elementType = pointerType->getElementType();

    return TypeWrapper::of(elementType);
}

Napi::Object PointerTypeWrapper::of(llvm::PointerType *type) {
    Napi::EscapableHandleScope escapeScope {};

    Napi::FunctionReference functionTemplate = Napi::New(env, pointerTypeTemplate());
    auto constructorFunction = Napi::GetFunction(functionTemplate);
    Napi::Value argv[1] = { Napi::External::New(env, type) };
    Napi::Object object = Napi::NewInstance(constructorFunction, 1, argv);

    return escapeScope.Escape(object);
}

Napi::FunctionReference& PointerTypeWrapper::pointerTypeTemplate() {
    static Napi::FunctionReference persistentTemplate {};

    if (persistentTemplate.IsEmpty()) {
        Napi::FunctionReference pointerTypeTemplate = Napi::Function::New(env, PointerTypeWrapper::New);

        Napi::SetMethod(pointerTypeTemplate, "get", PointerTypeWrapper::get);
        pointerTypeTemplate->SetClassName(Napi::String::New(env, "PointerType"));

        pointerTypeTemplate->Inherit(Napi::New(env, typeTemplate()));

        Napi::SetAccessor(pointerTypeTemplate->InstanceTemplate(), Napi::String::New(env, "elementType"), PointerTypeWrapper::getElementType);

        persistentTemplate.Reset(pointerTypeTemplate);
    }

    return persistentTemplate;
}

bool PointerTypeWrapper::isInstance(Napi::Value object) {
    Napi::Env env = object.Env();
    return Napi::New(env, pointerTypeTemplate())->HasInstance(object);
}

llvm::PointerType *PointerTypeWrapper::getPointerType() {
    return static_cast<llvm::PointerType*>(getType());
}

