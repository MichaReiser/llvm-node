//
// Created by Micha Reiser on 28.02.17.
//

#include "../util/string.h"
#include "data-layout.h"
#include "type.h"

Nan::Persistent<v8::FunctionTemplate> DataLayoutWrapper::functionTemplate {};

v8::Local<v8::Object> DataLayoutWrapper::of(llvm::DataLayout layout) {
    v8::Local<v8::FunctionTemplate> localFunctionTemplate = Nan::New(functionTemplate);
    v8::Local<v8::Object> object = Nan::NewInstance(localFunctionTemplate->InstanceTemplate()).ToLocalChecked();

    DataLayoutWrapper* wrapper = new DataLayoutWrapper(layout);
    wrapper->Wrap(object);

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(object);
}

NAN_MODULE_INIT(DataLayoutWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);

    tpl->SetClassName(Nan::New("DataLayout").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    Nan::SetPrototypeMethod(tpl, "getStringRepresentation", DataLayoutWrapper::getStringRepresentation);
    Nan::SetPrototypeMethod(tpl, "getPrefTypeAlignment", DataLayoutWrapper::getPrefTypeAlignment);
    Nan::SetPrototypeMethod(tpl, "getPointerSize", DataLayoutWrapper::getPointerSize);

    functionTemplate.Reset(tpl);

    Nan::Set(target, Nan::New("DataLayout").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(DataLayoutWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DataLayout functionTemplate needs to be called with new");
    }

    if (info.Length() < 1 && !info[0]->IsString()) {
        return Nan::ThrowTypeError("DataLayout functionTemplate needs to be called with single string argument");
    }

    llvm::DataLayout layout { ToString(info[0]->ToString()) };
    DataLayoutWrapper* wrapper = new DataLayoutWrapper { layout };
    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(DataLayoutWrapper::getStringRepresentation) {
    DataLayoutWrapper* wrapper = DataLayoutWrapper::FromValue(info.Holder());
    std::string representation = wrapper->layout.getStringRepresentation();

    info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), representation.c_str()));
}

NAN_METHOD(DataLayoutWrapper::getPointerSize) {
    if (info.Length() != 1 || !info[0]->IsUint32()) {
        return Nan::ThrowTypeError("getPointerSize needs to be called with: AS: uint32");
    }

    auto as = Nan::To<uint32_t>(info[0]).FromJust();
    auto dataLayout = DataLayoutWrapper::FromValue(info.Holder())->getDataLayout();
    info.GetReturnValue().Set(Nan::New(dataLayout.getPointerSize(as)));
}

NAN_METHOD(DataLayoutWrapper::getPrefTypeAlignment) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("getPrefTypeAlignment needs to be called with: type: Type");
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto dataLayout = DataLayoutWrapper::FromValue(info.Holder())->getDataLayout();

    info.GetReturnValue().Set(Nan::New(dataLayout.getPrefTypeAlignment(type)));
}

llvm::DataLayout DataLayoutWrapper::getDataLayout() {
    return layout;
}

bool DataLayoutWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate)->HasInstance(value);
}
