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

v8::Local<v8::Object> LLVMContextWrapper::of(llvm::LLVMContext &llvmContext) {
    auto constructorFunction = Nan::GetFunction(Nan::New(functionTemplate)).ToLocalChecked();
    v8::Local<v8::Value> argv[1] = { Nan::New<v8::External>(&llvmContext) };
    auto instance = Nan::NewInstance(constructorFunction, 1, argv).ToLocalChecked();

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

NAN_METHOD(LLVMContextWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("LLVMContext functionTemplate needs to be called with new");
    }

    auto* wrapper = new LLVMContextWrapper {};
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

llvm::LLVMContext &LLVMContextWrapper::getContext() {
    return context;
}

bool LLVMContextWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate)->HasInstance(value);
}
