//
// Created by Micha Reiser on 01.03.17.
//

#include <iostream>
#include <llvm/Config/llvm-config.h>
#include "argument.h"
#include "type.h"
#include "../util/string.h"
#include "function.h"

Napi::Object ArgumentWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::FunctionReference functionTemplate = Napi::Function::New(env, ArgumentWrapper::New);
    functionTemplate->SetClassName(Napi::String::New(env, "Argument"));

    functionTemplate->Inherit(Napi::New(env, ValueWrapper::valueTemplate()));

    Napi::SetAccessor(functionTemplate->InstanceTemplate(), Napi::String::New(env, "argumentNumber"), ArgumentWrapper::getArgNo);
    Napi::SetAccessor(functionTemplate->InstanceTemplate(), Napi::String::New(env, "parent"), ArgumentWrapper::getParent);
    Napi::SetPrototypeMethod(functionTemplate, "addAttr", ArgumentWrapper::addAttr);
    Napi::SetPrototypeMethod(functionTemplate, "hasAttribute", ArgumentWrapper::hasAttribute);
    Napi::SetPrototypeMethod(functionTemplate, "addDereferenceableAttr", ArgumentWrapper::addDereferenceableAttr);

    argumentTemplate().Reset(functionTemplate);

    (target).Set(Napi::String::New(env, "Argument"), Napi::GetFunction(functionTemplate));
}

Napi::Value ArgumentWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Class Constructor Argument cannot be invoked without new").ThrowAsJavaScriptException();
        return env.Null();
    }

    // Created using of
    if (info.Length() == 1 && info[0].IsExternal()) {
        llvm::Argument* argument = static_cast<llvm::Argument*>(*info[0].As<Napi::External>()->Value());
        auto* wrapper = new ArgumentWrapper { argument };
        wrapper->Wrap(info.This());

        return return info.This();
    }

    if (info.Length() < 1 || !TypeWrapper::isInstance(info[0])
        || (info.Length() > 1 && !info[1].IsString())
        || (info.Length() > 2 && !FunctionWrapper::isInstance(info[2]))
        || (info.Length() == 4 && !info[3].IsUint32())
        || info.Length() > 4) {
        Napi::TypeError::New(env, "Argument constructor requires: type: Type, name: string?, function: Function?, argNo: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    llvm::Function* function = nullptr;
    std::string name {};
    uint32_t argNo = 0;

    if (info.Length() > 1) {
        name = ToString(info[1]);
    }

    if (info.Length() > 2) {
        function = FunctionWrapper::FromValue(info[2])->getFunction();
    }

    if (info.Length() == 4) {
        argNo = info[3].As<Napi::Number>().Uint32Value();
    }

#if LLVM_VERSION_MAJOR == 4
    auto* argument = new llvm::Argument { type, name, function };
#else
    auto* argument = new llvm::Argument { type, name, function, argNo };
#endif
    auto* wrapper = new ArgumentWrapper { argument };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value ArgumentWrapper::getArgNo(const Napi::CallbackInfo& info) {
    auto* wrapper = ArgumentWrapper::FromValue(info.Holder());
    auto number = Napi::New(env, wrapper->getArgument()->getArgNo());

    return number;
}

Napi::Value ArgumentWrapper::getParent(const Napi::CallbackInfo& info) {
    auto* wrapper = ArgumentWrapper::FromValue(info.Holder());
    auto* function = wrapper->getArgument()->getParent();

    if (function) {
        return return FunctionWrapper::of(function);
    }
    return return env.Undefined();
}

Napi::Value ArgumentWrapper::addAttr(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !info[0].IsUint32()) {
        Napi::TypeError::New(env, "addAttr needs to be called with: attributeKind: Attribute.AttrKind").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* argument = ArgumentWrapper::FromValue(info.Holder())->getArgument();
    auto attributeKind = static_cast<llvm::Attribute::AttrKind>(info[0].As<Napi::Number>().Uint32Value());

    argument->addAttr(attributeKind);
}

Napi::Value ArgumentWrapper::hasAttribute(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !info[0].IsUint32()) {
        Napi::TypeError::New(env, "hasAttr needs to be called with: attributeKind: Attribute.AttrKind").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* argument = ArgumentWrapper::FromValue(info.Holder())->getArgument();
    auto attributeKind = static_cast<llvm::Attribute::AttrKind>(info[0].As<Napi::Number>().Uint32Value());

    return argument->hasAttribute(attributeKind);
}

Napi::Value ArgumentWrapper::addDereferenceableAttr(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !info[0].IsUint32()) {
        Napi::TypeError::New(env, "addDereferenceableAttr needs to be called with: bytes: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* argument = ArgumentWrapper::FromValue(info.Holder())->getArgument();
    auto bytes = info[0].As<Napi::Number>().Uint32Value();

    llvm::AttrBuilder builder {};
    builder.addDereferenceableAttr(bytes);

#if LLVM_VERSION_MAJOR == 4
    auto attributes = llvm::AttributeSet::get(argument->getContext(), argument->getArgNo() + 1, builder);
    argument->addAttr(attributes);
#else
    argument->addAttrs(builder);
#endif
}

llvm::Argument *ArgumentWrapper::getArgument() {
    return static_cast<llvm::Argument*>(ValueWrapper::getValue());
}

Napi::Object ArgumentWrapper::of(llvm::Argument *argument) {
    Napi::EscapableHandleScope escapeScope {};

    auto functionTemplate = Napi::New(env, argumentTemplate());
    auto constructorFunction = Napi::GetFunction(functionTemplate);
    Napi::Value args[1] = { Napi::External::New(env, argument) };
    auto instance = Napi::NewInstance(constructorFunction, 1, args);

    return escapeScope.Escape(instance);
}
