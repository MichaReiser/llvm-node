#include "di-subroutine-type.h"

Nan::Persistent<v8::FunctionTemplate> DISubroutineTypeWrapper::functionTemplate {};

NAN_MODULE_INIT(DISubroutineTypeWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("DISubroutineType").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->Inherit(Nan::New(DITypeWrapper::functionTemplate));

    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("parameters").ToLocalChecked(), DISubroutineTypeWrapper::getParameters);

    functionTemplate.Reset(tpl);

    Nan::Set(target, Nan::New("DISubroutineType").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}


NAN_METHOD(DISubroutineTypeWrapper::New) {
    if (!info.IsConstructCall())
    {
        return Nan::ThrowTypeError("Class Constructor Value cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal())
    {
        return Nan::ThrowTypeError("External Value Pointer required");
    }

    llvm::DISubroutineType *type = static_cast<llvm::DISubroutineType *>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DISubroutineTypeWrapper { type };

    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_GETTER(DISubroutineTypeWrapper::getParameters) {
    auto* wrapper = DISubroutineTypeWrapper::FromValue(info.Holder())->getDISubroutineType();

    unsigned size = wrapper->getTypeArray().size();
    auto result = Nan::New<v8::Array>(size);

    for (unsigned i = 0; i < size; i++) {
        result->Set(i, DITypeWrapper::of(wrapper->getTypeArray()[i]));
    }

    info.GetReturnValue().Set(result);
}

v8::Local<v8::Object> DISubroutineTypeWrapper::of(llvm::DISubroutineType *type) {
    v8::Local<v8::FunctionTemplate> localFunctionTemplate = Nan::New(functionTemplate);
    v8::Local<v8::Object> object = Nan::NewInstance(localFunctionTemplate->InstanceTemplate()).ToLocalChecked();

    DISubroutineTypeWrapper* wrapper = new DISubroutineTypeWrapper { type };
    wrapper->Wrap(object);

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(object);
}

llvm::DISubroutineType *DISubroutineTypeWrapper::getDISubroutineType() {
    return static_cast<llvm::DISubroutineType*>(getDIScope());
}

bool DISubroutineTypeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate)->HasInstance(value);
}
