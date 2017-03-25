//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-pointer-null.h"
#include "pointer-type.h"

NAN_MODULE_INIT(ConstantPointerNullWrapper::Init) {
    auto constantPointerNull = Nan::GetFunction(Nan::New(constantPointerNullTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("ConstantPointerNull").ToLocalChecked(), constantPointerNull);
}

NAN_METHOD(ConstantPointerNullWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor ConstantPointerNull cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("ConstantPointerNull constructor needs to be called with: constantPointerNull: external");
    }

    auto* constantPointerNull = static_cast<llvm::ConstantPointerNull*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ConstantPointerNullWrapper { constantPointerNull };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ConstantPointerNullWrapper::get) {
    if (info.Length() != 1 || !PointerTypeWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("get needs to be called with: type: PointerType");
    }

    auto* type = PointerTypeWrapper::FromValue(info[0])->getPointerType();
    auto* constant = llvm::ConstantPointerNull::get(type);

    info.GetReturnValue().Set(ConstantPointerNullWrapper::of(constant));
}

llvm::ConstantPointerNull *ConstantPointerNullWrapper::getConstantPointerNull() {
    return static_cast<llvm::ConstantPointerNull*>(getValue());
}

v8::Local<v8::Object> ConstantPointerNullWrapper::of(llvm::ConstantPointerNull *constantPointerNull) {
    auto constructorFunction = Nan::GetFunction(Nan::New(constantPointerNullTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(constantPointerNull) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe {};
    return escapeScpoe.Escape(instance);
}

Nan::Persistent<v8::FunctionTemplate>& ConstantPointerNullWrapper::constantPointerNullTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(ConstantPointerNullWrapper::New);
        localTemplate->SetClassName(Nan::New("ConstantPointerNull").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(constantTemplate()));

        Nan::SetMethod(localTemplate, "get", ConstantPointerNullWrapper::get);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}