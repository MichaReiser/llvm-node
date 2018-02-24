//
// Created by Micha Reiser on 28.02.17.
//

#include "llvm-context.h"

Napi::FunctionReference LLVMContextWrapper::constructor;

Napi::Object LLVMContextWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope{env};

    Napi::Function func = DefineClass(env, "LLVMContext", {

    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("LLVMContext", func);
    return exports;
}

LLVMContextWrapper::LLVMContextWrapper(const Napi::CallbackInfo &info): Napi::ObjectWrap<LLVMContextWrapper>(info), context {} {
}

llvm::LLVMContext &LLVMContextWrapper::getContext() {
    return context;
}

bool LLVMContextWrapper::isInstanceOfType(const Napi::Value &value) {
    return value.As<Napi::Object>().InstanceOf(constructor.Value());
}
