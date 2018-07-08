//
// Created by Micha Reiser on 28.02.17.
//

#include "target-machine.h"
//#include "../ir/data-layout.h"

Napi::FunctionReference TargetMachineWrapper::constructor;

void TargetMachineWrapper::Init(Napi::Env env, Napi::Object& exports) {
    Napi::HandleScope scope { env };

    Napi::Function func = DefineClass(env, "TargetMachine", {
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("TargetMachine", func);
}

Napi::Object TargetMachineWrapper::of(Napi::Env env, llvm::TargetMachine* targetMachine) {
    Napi::Object result = constructor.New({Napi::External<llvm::TargetMachine>::New(env, targetMachine)});

    Napi::EscapableHandleScope escapeScope{env};
    return escapeScope.Escape(result).ToObject();
}

bool TargetMachineWrapper::isInstanceOfType(const Napi::Value &value) {
    return value.ToObject().InstanceOf(constructor.Value());
}

TargetMachineWrapper::TargetMachineWrapper(const Napi::CallbackInfo &info): Napi::ObjectWrap<TargetMachineWrapper>{info} {
    Napi::Env env = info.Env();
    Napi::HandleScope scope { env };
    size_t argsLength = info.Length();

    if (!info.IsConstructCall()) {
        throw Napi::TypeError::New(env, "TargetMachine constructor needs to be called with new");
    }

    if (argsLength < 1 || !info[0].IsExternal()) {
        throw Napi::TypeError::New(env, "TargetMachine expected");
    }

    auto external = info[0].As<Napi::External<llvm::TargetMachine>>();
    this->targetMachine = external.Data();
}


// TODO createDataLayout
//Napi::Value TargetMachineWrapper::createDataLayout(const Napi::CallbackInfo& info) {
//    auto dataLayout = TargetMachineWrapper::FromValue(info.Holder())->targetMachine->createDataLayout();
//    return DataLayoutWrapper::of(dataLayout);
//}


