#include "di-type.h"

Nan::Persistent<v8::FunctionTemplate> DITypeWrapper::functionTemplate {};

NAN_MODULE_INIT(DITypeWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("DIType").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->Inherit(Nan::New(DIScopeWrapper::functionTemplate));

    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("line").ToLocalChecked(), DITypeWrapper::getLine);

    functionTemplate.Reset(tpl);

    Nan::Set(target, Nan::New("DIType").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}


NAN_METHOD(DITypeWrapper::New) {
    if (!info.IsConstructCall())
    {
        return Nan::ThrowTypeError("Class Constructor Value cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal())
    {
        return Nan::ThrowTypeError("External Value Pointer required");
    }

    llvm::DIType *type = static_cast<llvm::DIType *>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DITypeWrapper { type };

    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_GETTER(DITypeWrapper::getLine) {
    auto* wrapper = DITypeWrapper::FromValue(info.Holder())->getDIType();
    info.GetReturnValue().Set(Nan::New(wrapper->getLine()));
}

v8::Local<v8::Object> DITypeWrapper::of(llvm::DIType *type) {
    v8::Local<v8::FunctionTemplate> localFunctionTemplate = Nan::New(functionTemplate);
    v8::Local<v8::Object> object = Nan::NewInstance(localFunctionTemplate->InstanceTemplate()).ToLocalChecked();

    DITypeWrapper* wrapper = new DITypeWrapper { type };
    wrapper->Wrap(object);

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(object);
}

llvm::DIType *DITypeWrapper::getDIType() {
    return static_cast<llvm::DIType*>(getDIScope());
}

bool DITypeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate)->HasInstance(value);
}
