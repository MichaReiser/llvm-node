//
// Created by Micha Reiser on 28.02.17.
//

#include "llvm-context.h"

Nan::Persistent<v8::FunctionTemplate> LLVMContextWrapper::functionTemplate {};

NAN_MODULE_INIT(LLVMContextWrapper::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("LLVMContext").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    functionTemplate.Reset(tpl);
    Nan::Set(target, Nan::New("LLVMContext").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(LLVMContextWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("LLVMContext functionTemplate needs to be called with new");
    }

    LLVMContextWrapper* wrapper = new LLVMContextWrapper {};
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

llvm::LLVMContext &LLVMContextWrapper::get() {
    return context;
}

bool LLVMContextWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate)->HasInstance(value);
}
