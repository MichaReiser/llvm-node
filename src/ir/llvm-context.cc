//
// Created by Micha Reiser on 28.02.17.
//

#include "llvm-context.h"

Napi::FunctionReference LLVMContextWrapper::functionTemplate {};

Napi::Object LLVMContextWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::FunctionReference tpl = Napi::Function::New(env, New);
    tpl->SetClassName(Napi::String::New(env, "LLVMContext"));


    functionTemplate.Reset(tpl);
    (target).Set(Napi::String::New(env, "LLVMContext"), Napi::GetFunction(tpl));
}

Napi::Object LLVMContextWrapper::of(llvm::LLVMContext &llvmContext) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, functionTemplate));
    Napi::Value argv[1] = { Napi::External::New(env, &llvmContext) };
    auto instance = Napi::NewInstance(constructorFunction, 1, argv);

    Napi::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

Napi::Value LLVMContextWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "LLVMContext functionTemplate needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* wrapper = new LLVMContextWrapper {};
    wrapper->Wrap(info.This());

    return info.This();
}

llvm::LLVMContext &LLVMContextWrapper::getContext() {
    return context;
}

bool LLVMContextWrapper::isInstance(Napi::Value value) {
    Napi::Env env = value.Env();
    return Napi::New(env, functionTemplate)->HasInstance(value);
}
