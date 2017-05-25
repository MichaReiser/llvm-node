//
// Created by Micha Reiser on 24.05.17.
//

#include "undef-value.h"
#include "llvm-context.h"
#include "type.h"

NAN_MODULE_INIT(UndefValueWrapper::Init) {
    auto undefValue = Nan::GetFunction(Nan::New(undefValueTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("UndefValue").ToLocalChecked(), undefValue);
}

NAN_METHOD(UndefValueWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor UndefValueWrapper cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("UndefValueWrapper constructor needs to be called with: undefValueWrapper: external");
    }

    auto* undefValue = static_cast<llvm::UndefValue*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new UndefValueWrapper { undefValue };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(UndefValueWrapper::get) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("get needs to be called with: type: Type");
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto* undefValue = llvm::UndefValue::get(type);
    info.GetReturnValue().Set(UndefValueWrapper::of(undefValue));
}

llvm::UndefValue* UndefValueWrapper::getUndefValue() {
    return static_cast<llvm::UndefValue*>(getValue());
}

v8::Local<v8::Object> UndefValueWrapper::of(llvm::UndefValue* undefValue) {
    auto constructorFunction = Nan::GetFunction(Nan::New(undefValueTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(undefValue) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe {};
    return escapeScpoe.Escape(instance);
}

Nan::Persistent<v8::FunctionTemplate>& UndefValueWrapper::undefValueTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(UndefValueWrapper::New);
        localTemplate->SetClassName(Nan::New("UndefValue").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(constantTemplate()));

        Nan::SetMethod(localTemplate, "get", UndefValueWrapper::get);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}