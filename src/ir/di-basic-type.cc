#include "di-basic-type.h"

Nan::Persistent<v8::FunctionTemplate> DIBasicTypeWrapper::functionTemplate {};

NAN_MODULE_INIT(DIBasicTypeWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("DIBasicType").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->Inherit(Nan::New(DITypeWrapper::functionTemplate));

    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("isSigned").ToLocalChecked(), DIBasicTypeWrapper::getSignedness);

    functionTemplate.Reset(tpl);

    Nan::Set(target, Nan::New("DIBasicType").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}


NAN_METHOD(DIBasicTypeWrapper::New) {
    if (!info.IsConstructCall())
    {
        return Nan::ThrowTypeError("Class Constructor Value cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal())
    {
        return Nan::ThrowTypeError("External Value Pointer required");
    }

    llvm::DIBasicType *type = static_cast<llvm::DIBasicType *>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIBasicTypeWrapper { type };

    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_GETTER(DIBasicTypeWrapper::getSignedness) {
    auto* wrapper = DIBasicTypeWrapper::FromValue(info.Holder())->getDIBasicType();
    bool isSigned = wrapper->getSignedness() == llvm::DIBasicType::Signedness::Signed;
    info.GetReturnValue().Set(Nan::New(isSigned));
}

v8::Local<v8::Object> DIBasicTypeWrapper::of(llvm::DIBasicType *type) {
    v8::Local<v8::FunctionTemplate> localFunctionTemplate = Nan::New(functionTemplate);
    v8::Local<v8::Object> object = Nan::NewInstance(localFunctionTemplate->InstanceTemplate()).ToLocalChecked();

    DIBasicTypeWrapper* wrapper = new DIBasicTypeWrapper { type };
    wrapper->Wrap(object);

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(object);
}

llvm::DIBasicType *DIBasicTypeWrapper::getDIBasicType() {
    return static_cast<llvm::DIBasicType*>(getDIScope());
}

bool DIBasicTypeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate)->HasInstance(value);
}
