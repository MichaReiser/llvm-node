//
// Created by Micha Reiser on 28.02.17.
//

#include "target-machine.h"
#include "../ir/data-layout.h"


Napi::Persistent<v8::ObjectTemplate> TargetMachineWrapper::targetMachineTemplate {};

Napi::Object TargetMachineWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto objectTemplate = Napi::ObjectTemplate::New(env);


    Napi::SetMethod(objectTemplate, "createDataLayout", TargetMachineWrapper::createDataLayout);

    targetMachineTemplate.Reset(objectTemplate);
}

Napi::Value TargetMachineWrapper::createDataLayout(const Napi::CallbackInfo& info) {
    auto dataLayout = TargetMachineWrapper::FromValue(info.Holder())->targetMachine->createDataLayout();
    return DataLayoutWrapper::of(dataLayout);
}

Napi::Object TargetMachineWrapper::of(const llvm::TargetMachine *targetMachinePtr) {
    Napi::EscapableHandleScope escapeScope {};
    v8::Local<v8::ObjectTemplate> tpl = Napi::New(env, targetMachineTemplate);

    auto object = Napi::NewInstance(tpl);
    auto* wrapper = new TargetMachineWrapper { targetMachinePtr };
    wrapper->Wrap(object);

    return escapeScope.Escape(object);
}

