#include "di-file.h"

Nan::Persistent<v8::FunctionTemplate> DIFileWrapper::functionTemplate {};

NAN_MODULE_INIT(DIFileWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("DIFile").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("filename").ToLocalChecked(), DIFileWrapper::getFilename);
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("directory").ToLocalChecked(), DIFileWrapper::getDirectory);

    functionTemplate.Reset(tpl);

    Nan::Set(target, Nan::New("DIFile").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}


NAN_METHOD(DIFileWrapper::New) {
    if (!info.IsConstructCall())
    {
        return Nan::ThrowTypeError("Class Constructor Value cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal())
    {
        return Nan::ThrowTypeError("External Value Pointer required");
    }

    llvm::DIFile *file = static_cast<llvm::DIFile *>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIFileWrapper { file };

    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_GETTER(DIFileWrapper::getFilename) {
    auto* wrapper = DIFileWrapper::FromValue(info.Holder())->getDIFile();
    auto result = Nan::New(wrapper->getFilename().str()).ToLocalChecked();
    info.GetReturnValue().Set(result);
}

NAN_GETTER(DIFileWrapper::getDirectory) {
    auto* wrapper = DIFileWrapper::FromValue(info.Holder())->getDIFile();
    auto result = Nan::New(wrapper->getDirectory().str()).ToLocalChecked();
    info.GetReturnValue().Set(result);
}

v8::Local<v8::Object> DIFileWrapper::of(llvm::DIFile *file) {
    v8::Local<v8::FunctionTemplate> localFunctionTemplate = Nan::New(functionTemplate);
    v8::Local<v8::Object> object = Nan::NewInstance(localFunctionTemplate->InstanceTemplate()).ToLocalChecked();

    DIFileWrapper* wrapper = new DIFileWrapper { file };
    wrapper->Wrap(object);

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(object);
}

llvm::DIFile *DIFileWrapper::getDIFile() {
    return this->diFile;
}

bool DIFileWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate)->HasInstance(value);
}
