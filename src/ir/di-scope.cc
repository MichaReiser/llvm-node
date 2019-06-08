#include "di-scope.h"

Nan::Persistent<v8::FunctionTemplate> DIScopeWrapper::functionTemplate {};

NAN_MODULE_INIT(DIScopeWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("DIScope").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("filename").ToLocalChecked(), DIScopeWrapper::getFilename);
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("directory").ToLocalChecked(), DIScopeWrapper::getDirectory);
    Nan::SetAccessor(tpl->InstanceTemplate(), Nan::New("name").ToLocalChecked(), DIScopeWrapper::getName);

    functionTemplate.Reset(tpl);

    Nan::Set(target, Nan::New("DIScope").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}


NAN_METHOD(DIScopeWrapper::New) {
    if (!info.IsConstructCall())
    {
        return Nan::ThrowTypeError("Class Constructor Value cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal())
    {
        return Nan::ThrowTypeError("External Value Pointer required");
    }

    llvm::DIScope *scope = static_cast<llvm::DIScope *>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new DIScopeWrapper { scope };

    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_GETTER(DIScopeWrapper::getFilename) {
    auto* wrapper = DIScopeWrapper::FromValue(info.Holder())->getDIScope();
    auto result = Nan::New(wrapper->getFilename().str()).ToLocalChecked();

    info.GetReturnValue().Set(result);
}

NAN_GETTER(DIScopeWrapper::getDirectory) {
    auto* wrapper = DIScopeWrapper::FromValue(info.Holder())->getDIScope();
    auto result = Nan::New(wrapper->getDirectory().str()).ToLocalChecked();

    info.GetReturnValue().Set(result);
}

NAN_GETTER(DIScopeWrapper::getName) {
    auto* wrapper = DIScopeWrapper::FromValue(info.Holder())->getDIScope();
    auto result = Nan::New(wrapper->getName().str()).ToLocalChecked();

    info.GetReturnValue().Set(result);
}

v8::Local<v8::Object> DIScopeWrapper::of(llvm::DIScope *scope) {
    v8::Local<v8::FunctionTemplate> localFunctionTemplate = Nan::New(functionTemplate);
    v8::Local<v8::Object> object = Nan::NewInstance(localFunctionTemplate->InstanceTemplate()).ToLocalChecked();

    DIScopeWrapper* wrapper = new DIScopeWrapper { scope };
    wrapper->Wrap(object);

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(object);
}

llvm::DIScope *DIScopeWrapper::getDIScope() {
    return this->scope;
}

bool DIScopeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate)->HasInstance(value);
}
