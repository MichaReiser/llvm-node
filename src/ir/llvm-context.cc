//
// Created by Micha Reiser on 28.02.17.
//

#include "llvm-context.h"

Napi::FunctionReference LLVMContextWrapper::constructor;

void LLVMContextWrapper::Init(Napi::Env env, Napi::Object &exports){
    Napi::HandleScope scope { env };

    Napi::Function func = DefineClass(env, "LLVMContext", {

    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("LLVMContext", func);
}

LLVMContextWrapper::LLVMContextWrapper(const Napi::CallbackInfo &info): Napi::ObjectWrap<LLVMContextWrapper> { info }, context {} {
    auto env = info.Env();

    if (!info.IsConstructCall()) {
        throw Napi::TypeError::New(env, "LLVMContext functionTemplate needs to be called with new");
    }
}

llvm::LLVMContext &LLVMContextWrapper::getContext() {
    return context;
}

bool LLVMContextWrapper::isInstanceOfType(const Napi::Value &value) {
    return value.As<Napi::Object>().InstanceOf(constructor.Value());
}
