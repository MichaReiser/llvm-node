//
// Created by Micha Reiser on 01.03.17.
//

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
        || (info.Length() == 3 && !FunctionWrapper::isInstance(info[2]))
        || info.Length() > 3) {
        return Nan::ThrowTypeError("Argument constructor requires: type: Type, name: string?, function: Function?");
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    llvm::Function* function = nullptr;
    std::string name {};

    if (info.Length() > 1) {
        name = ToString(Nan::To<v8::String>(info[1]).ToLocalChecked());
    }

    if (info.Length() == 3) {
        function = FunctionWrapper::FromValue(info[2])->getFunction();
    }

    auto* argument = new llvm::Argument { type, name, function };
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
