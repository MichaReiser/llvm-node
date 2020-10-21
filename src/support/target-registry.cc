#include "../util/string.h"
#include "target-registry.h"
#include "../target/target-machine.h"

NAN_METHOD(TargetRegistryWrapper::lookupTarget) {
    if (info.Length() == 1 && info[0]->IsString()) {
        std::string triple = ToString(Nan::To<v8::String>(info[0]).ToLocalChecked());
        std::string error {};

        const llvm::Target* result = llvm::TargetRegistry::lookupTarget(triple, error);

        if (!result) {
            std::string msg = "Failed to lookup target '" + triple + "': " + error;
            Nan::ThrowError(msg.c_str());
            return;
        }

        info.GetReturnValue().Set(TargetWrapper::of(result));
    } else {
        Nan::ThrowTypeError("lookupTarget needs to be called with a single string argument");
    }
}

NAN_MODULE_INIT(TargetRegistryWrapper::Init) {
    v8::Local<v8::ObjectTemplate> tpl = Nan::New<v8::ObjectTemplate>();
    Nan::SetMethod(tpl, "lookupTarget", TargetRegistryWrapper::lookupTarget);

    auto targetRegistry = Nan::NewInstance(tpl).ToLocalChecked();

    Nan::Set(target, Nan::New("TargetRegistry").ToLocalChecked(), targetRegistry);
}

//--------------------------------------------------------------
// TargetWrapper
//--------------------------------------------------------------

Nan::Persistent<v8::ObjectTemplate> TargetWrapper::target_template {};

NAN_MODULE_INIT(TargetWrapper::Init) {
    v8::Local<v8::ObjectTemplate> tpl = Nan::New<v8::ObjectTemplate>();
    tpl->SetInternalFieldCount(1);

    Nan::SetMethod(tpl, "createTargetMachine", TargetWrapper::createTargetMachine);
    Nan::SetAccessor(tpl, Nan::New("name").ToLocalChecked(), TargetWrapper::getName);
    Nan::SetAccessor(tpl, Nan::New("getShortDescription").ToLocalChecked(), TargetWrapper::getShortDescription);

    target_template.Reset(tpl);
}

NAN_METHOD(TargetWrapper::createTargetMachine) {
    if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsString()) {
        return Nan::ThrowError("Function needs to be called at least with the arguments: triple: string, cpu: string.");
    }

    std::string targetTriple = ToString(Nan::To<v8::String>(info[0]).ToLocalChecked());
    std::string cpu = ToString(Nan::To<v8::String>(info[1]).ToLocalChecked());
    std::string features {};

    if (info.Length() > 2) {
        if (info[2]->IsString()) {
            features = ToString(Nan::To<v8::String>(info[2]).ToLocalChecked());
        } else {
            return Nan::ThrowError("The 3th argument of createTargetMachine needs to be a string");
        }
    }

    llvm::TargetOptions options {};
    TargetWrapper* wrapper = TargetWrapper::FromValue(info.Holder());

    auto* targetMachinePtr = wrapper->target->createTargetMachine(targetTriple, cpu, features, options, llvm::Optional<llvm::Reloc::Model> {});
    info.GetReturnValue().Set(TargetMachineWrapper::of(targetMachinePtr));
}

NAN_GETTER(TargetWrapper::getName) {
    TargetWrapper* wrapper = TargetWrapper::FromValue(info.Holder());
    auto result = Nan::MakeMaybe(v8::String::NewFromUtf8(info.GetIsolate(), wrapper->target->getName())).ToLocalChecked();
    info.GetReturnValue().Set(result);
}

NAN_GETTER(TargetWrapper::getShortDescription) {
    TargetWrapper* wrapper = TargetWrapper::FromValue(info.Holder());
    auto result = Nan::MakeMaybe(v8::String::NewFromUtf8(info.GetIsolate(), wrapper->target->getShortDescription())).ToLocalChecked();
    info.GetReturnValue().Set(result);
}

v8::Local<v8::Object> TargetWrapper::of(const llvm::Target *llvmTarget) {
    Nan::EscapableHandleScope escapeScope {};

    v8::Local<v8::ObjectTemplate> tpl = Nan::New(target_template);
    auto object = Nan::NewInstance(tpl).ToLocalChecked();

    auto wrapper = new TargetWrapper { llvmTarget };
    wrapper->Wrap(object);

    return escapeScope.Escape(object);
}
