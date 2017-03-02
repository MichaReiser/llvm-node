//
// Created by Micha Reiser on 28.02.17.
//

#include "target-machine.h"
#include "../ir/data-layout.h"


Nan::Persistent<v8::ObjectTemplate> TargetMachineWrapper::targetMachineTemplate {};

NAN_MODULE_INIT(TargetMachineWrapper::Init) {
    auto objectTemplate = Nan::New<v8::ObjectTemplate>();
    objectTemplate->SetInternalFieldCount(1);

    Nan::SetMethod(objectTemplate, "createDataLayout", TargetMachineWrapper::createDataLayout);

    targetMachineTemplate.Reset(objectTemplate);
}

NAN_METHOD(TargetMachineWrapper::createDataLayout) {
    auto dataLayout = TargetMachineWrapper::FromValue(info.Holder())->targetMachine->createDataLayout();
    info.GetReturnValue().Set(DataLayoutWrapper::of(dataLayout));
}

v8::Local<v8::Object> TargetMachineWrapper::of(const llvm::TargetMachine *targetMachinePtr) {
    Nan::EscapableHandleScope escapeScope {};
    v8::Local<v8::ObjectTemplate> tpl = Nan::New(targetMachineTemplate);

    auto object = Nan::NewInstance(tpl).ToLocalChecked();
    auto* wrapper = new TargetMachineWrapper { targetMachinePtr };
    wrapper->Wrap(object);

    return escapeScope.Escape(object);
}

