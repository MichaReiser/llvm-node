#include <iostream>
#include "di-compile-unit.h"

Nan::Persistent<v8::FunctionTemplate> DICompileUnitWrapper::functionTemplate {};

NAN_MODULE_INIT(DICompileUnitWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("DICompileUnit").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    functionTemplate.Reset(tpl);

    Nan::Set(target, Nan::New("DICompileUnit").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(DICompileUnitWrapper::New) {
    if (!info.IsConstructCall())
    {
        return Nan::ThrowTypeError("Class Constructor Value cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal())
    {
        return Nan::ThrowTypeError("External Value Pointer required");
    }

    auto *file = static_cast<llvm::DICompileUnit *>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DICompileUnitWrapper { file };

    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> DICompileUnitWrapper::of(llvm::DICompileUnit *compileUnit) {
    v8::Local<v8::FunctionTemplate> localFunctionTemplate = Nan::New(functionTemplate);
    v8::Local<v8::Object> object = Nan::NewInstance(localFunctionTemplate->InstanceTemplate()).ToLocalChecked();

    DICompileUnitWrapper* wrapper = new DICompileUnitWrapper { compileUnit };
    wrapper->Wrap(object);

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(object);
}

llvm::DICompileUnit *DICompileUnitWrapper::getDICompileUnit() {
    return this->compileUnit;
}

bool DICompileUnitWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate)->HasInstance(value);
}
