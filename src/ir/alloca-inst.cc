//
// Created by Micha Reiser on 03.03.17.
//

#include "alloca-inst.h"
#include "type.h"

Napi::Object AllocaInstWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto allocaInst = Napi::GetFunction(Napi::New(env, allocaInstTemplate()));
    (target).Set(Napi::String::New(env, "AllocaInst"), allocaInst);
}

Napi::Object AllocaInstWrapper::of(llvm::AllocaInst* inst) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, allocaInstTemplate()));
    Napi::Value args[1] = { Napi::External::New(env, inst) };
    auto instance = Napi::NewInstance(constructorFunction, 1, args);

    Napi::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

llvm::AllocaInst* AllocaInstWrapper::getAllocaInst() {
    return static_cast<llvm::AllocaInst*>(getValue());
}

Napi::Value AllocaInstWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Class Constructor AllocaInst cannot be invoked without new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "AllocaInst constructor needs to be called with: inst: External").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* inst = static_cast<llvm::AllocaInst*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new AllocaInstWrapper { inst };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value AllocaInstWrapper::getAllocatedType(const Napi::CallbackInfo& info) {
    auto* wrapper = AllocaInstWrapper::FromValue(info.Holder());
    auto* type = wrapper->getAllocaInst()->getAllocatedType();

    return TypeWrapper::of(type);
}

void AllocaInstWrapper::setAllocatedType(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!TypeWrapper::isInstance(value)) {
        Napi::TypeError::New(env, "allocatedType needs to be a Type").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(value)->getType();
    auto* wrapper = AllocaInstWrapper::FromValue(info.Holder());
    wrapper->getAllocaInst()->setAllocatedType(type);
}

Napi::Value AllocaInstWrapper::getAlignment(const Napi::CallbackInfo& info) {
    auto* wrapper = AllocaInstWrapper::FromValue(info.Holder());

    return Napi::New(env, wrapper->getAllocaInst()->getAlignment());
}

void AllocaInstWrapper::setAlignment(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!value->IsUint32()) {
        Napi::TypeError::New(env, "alignment needs to be an uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* wrapper = AllocaInstWrapper::FromValue(info.Holder());
    wrapper->getAllocaInst()->setAlignment(value.As<Napi::Number>().Uint32Value());
}

Napi::Value AllocaInstWrapper::getArraySize(const Napi::CallbackInfo& info) {
    auto* wrapper = AllocaInstWrapper::FromValue(info.Holder());

    llvm::Value* arraySize = wrapper->getAllocaInst()->getArraySize();
    if (arraySize == nullptr) {
        return env.Null();
    } else {
        return ValueWrapper::of(arraySize);
    }
}

Napi::FunctionReference& AllocaInstWrapper::allocaInstTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Napi::Function::New(env, AllocaInstWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "AllocaInst"));
        localTemplate->Inherit(Napi::New(env, valueTemplate()));


        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "allocatedType"), AllocaInstWrapper::getAllocatedType, AllocaInstWrapper::setAllocatedType);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "alignment"), AllocaInstWrapper::getAlignment, AllocaInstWrapper::setAlignment);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "arraySize"), AllocaInstWrapper::getArraySize);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}