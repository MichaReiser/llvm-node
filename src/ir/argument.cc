//
// Created by Micha Reiser on 01.03.17.
//

#include <iostream>
#include <llvm/Config/llvm-config.h>
#include "argument.h"
#include "type.h"
#include "../util/string.h"
#include "function.h"

NAN_MODULE_INIT(ArgumentWrapper::Init) {
    v8::Local<v8::FunctionTemplate> functionTemplate = Nan::New<v8::FunctionTemplate>(ArgumentWrapper::New);
    functionTemplate->SetClassName(Nan::New("Argument").ToLocalChecked());
    functionTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    functionTemplate->Inherit(Nan::New(ValueWrapper::valueTemplate()));

    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("argumentNumber").ToLocalChecked(), ArgumentWrapper::getArgNo);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("parent").ToLocalChecked(), ArgumentWrapper::getParent);
    Nan::SetPrototypeMethod(functionTemplate, "addAttr", ArgumentWrapper::addAttr);
    Nan::SetPrototypeMethod(functionTemplate, "hasAttribute", ArgumentWrapper::hasAttribute);
    Nan::SetPrototypeMethod(functionTemplate, "addDereferenceableAttr", ArgumentWrapper::addDereferenceableAttr);

    argumentTemplate().Reset(functionTemplate);

    Nan::Set(target, Nan::New("Argument").ToLocalChecked(), Nan::GetFunction(functionTemplate).ToLocalChecked());
}

NAN_METHOD(ArgumentWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor Argument cannot be invoked without new");
    }

    // Created using of
    if (info.Length() == 1 && info[0]->IsExternal()) {
        llvm::Argument* argument = static_cast<llvm::Argument*>(v8::External::Cast(*info[0])->Value());
        auto* wrapper = new ArgumentWrapper { argument };
        wrapper->Wrap(info.This());

        return info.GetReturnValue().Set(info.This());
    }

    if (info.Length() < 1 || !TypeWrapper::isInstance(info[0])
        || (info.Length() > 1 && !info[1]->IsString())
        || (info.Length() > 2 && !FunctionWrapper::isInstance(info[2]))
        || (info.Length() == 4 && !info[3]->IsUint32())
        || info.Length() > 4) {
        return Nan::ThrowTypeError("Argument constructor requires: type: Type, name: string?, function: Function?, argNo: uint32");
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
        argNo = Nan::To<uint32_t>(info[3]).FromJust();
    }

#if LLVM_VERSION_MAJOR == 4
    auto* argument = new llvm::Argument { type, name, function };
#else
    auto* argument = new llvm::Argument { type, name, function, argNo };
#endif
    auto* wrapper = new ArgumentWrapper { argument };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_GETTER(ArgumentWrapper::getArgNo) {
    auto* wrapper = ArgumentWrapper::FromValue(info.Holder());
    auto number = Nan::New(wrapper->getArgument()->getArgNo());

    info.GetReturnValue().Set(number);
}

NAN_GETTER(ArgumentWrapper::getParent) {
    auto* wrapper = ArgumentWrapper::FromValue(info.Holder());
    auto* function = wrapper->getArgument()->getParent();

    if (function) {
        return info.GetReturnValue().Set(FunctionWrapper::of(function));
    }
    return info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(ArgumentWrapper::addAttr) {
    if (info.Length() != 1 || !info[0]->IsUint32()) {
        return Nan::ThrowTypeError("addAttr needs to be called with: attributeKind: Attribute.AttrKind");
    }

    auto* argument = ArgumentWrapper::FromValue(info.Holder())->getArgument();
    auto attributeKind = static_cast<llvm::Attribute::AttrKind>(Nan::To<uint32_t>(info[0]).FromJust());

    argument->addAttr(attributeKind);
}

NAN_METHOD(ArgumentWrapper::hasAttribute) {
    if (info.Length() != 1 || !info[0]->IsUint32()) {
        return Nan::ThrowTypeError("hasAttr needs to be called with: attributeKind: Attribute.AttrKind");
    }

    auto* argument = ArgumentWrapper::FromValue(info.Holder())->getArgument();
    auto attributeKind = static_cast<llvm::Attribute::AttrKind>(Nan::To<uint32_t>(info[0]).FromJust());

    info.GetReturnValue().Set(argument->hasAttribute(attributeKind));
}

NAN_METHOD(ArgumentWrapper::addDereferenceableAttr) {
    if (info.Length() != 1 || !info[0]->IsUint32()) {
        return Nan::ThrowTypeError("addDereferenceableAttr needs to be called with: bytes: uint32");
    }

    auto* argument = ArgumentWrapper::FromValue(info.Holder())->getArgument();
    auto bytes = Nan::To<uint32_t>(info[0]).FromJust();

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

v8::Local<v8::Object> ArgumentWrapper::of(llvm::Argument *argument) {
    Nan::EscapableHandleScope escapeScope {};

    auto functionTemplate = Nan::New(argumentTemplate());
    auto constructorFunction = Nan::GetFunction(functionTemplate).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(argument) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    return escapeScope.Escape(instance);
}
