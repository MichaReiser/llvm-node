#include "../util/string.h"
#include "target-registry.h"
#include "../target/target-machine.h"

Napi::Value TargetRegistryWrapper::lookupTarget(const Napi::CallbackInfo& info) {
    if (info.Length() == 1 && info[0].IsString()) {
        std::string triple = ToString(info[0].To<Napi::String>());
        std::string error {};

        const llvm::Target* result = llvm::TargetRegistry::lookupTarget(triple, error);

        if (!result) {
            std::string msg = "Failed to lookup target: " + error;
            Napi::Error::New(env, msg.c_str()).ThrowAsJavaScriptException();
            return env.Null();
        }

        return TargetWrapper::of(result);
    } else {
        Napi::TypeError::New(env, "lookupTarget needs to be called with a single string argument").ThrowAsJavaScriptException();

    }
}

Napi::Object TargetRegistryWrapper::Init(Napi::Env env, Napi::Object exports) {
    v8::Local<v8::ObjectTemplate> tpl = Napi::ObjectTemplate::New(env);
    Napi::SetMethod(tpl, "lookupTarget", TargetRegistryWrapper::lookupTarget);
    
    (target).Set(Napi::String::New(env, "TargetRegistry"), tpl->NewInstance());
}

//--------------------------------------------------------------
// TargetWrapper
//--------------------------------------------------------------

Napi::Persistent<v8::ObjectTemplate> TargetWrapper::target_template {};

Napi::Object TargetWrapper::Init(Napi::Env env, Napi::Object exports) {
    v8::Local<v8::ObjectTemplate> tpl = Napi::ObjectTemplate::New(env);


    Napi::SetMethod(tpl, "createTargetMachine", TargetWrapper::createTargetMachine);
    Napi::SetAccessor(tpl, Napi::String::New(env, "name"), TargetWrapper::getName);
    Napi::SetAccessor(tpl, Napi::String::New(env, "getShortDescription"), TargetWrapper::getShortDescription);

    target_template.Reset(tpl);
}

Napi::Value TargetWrapper::createTargetMachine(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
        Napi::Error::New(env, "Function needs to be called at least with the arguments: triple: string, cpu: string.").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string targetTriple = ToString(info[0].To<Napi::String>());
    std::string cpu = ToString(info[1].To<Napi::String>());
    std::string features {};

    if (info.Length() > 2) {
        if (info[2].IsString()) {
            features = ToString(info[2].To<Napi::String>());
        } else {
            Napi::Error::New(env, "The 3th argument of createTargetMachine needs to be a string").ThrowAsJavaScriptException();
            return env.Null();
        }
    }

    llvm::TargetOptions options {};
    TargetWrapper* wrapper = TargetWrapper::FromValue(info.Holder());

    auto* targetMachinePtr = wrapper->target->createTargetMachine(targetTriple, cpu, features, options, llvm::Optional<llvm::Reloc::Model> {});
    return TargetMachineWrapper::of(targetMachinePtr);
}

Napi::Value TargetWrapper::getName(const Napi::CallbackInfo& info) {
    TargetWrapper* wrapper = TargetWrapper::FromValue(info.Holder());
    auto result = v8::String::NewFromUtf8(info.GetIsolate(), wrapper->target->getName());
    return result;
}

Napi::Value TargetWrapper::getShortDescription(const Napi::CallbackInfo& info) {
    TargetWrapper* wrapper = TargetWrapper::FromValue(info.Holder());
    auto result = v8::String::NewFromUtf8(info.GetIsolate(), wrapper->target->getShortDescription());
    return result;
}

Napi::Object TargetWrapper::of(const llvm::Target *llvmTarget) {
    Napi::EscapableHandleScope escapeScope {};

    v8::Local<v8::ObjectTemplate> tpl = Napi::New(env, target_template);
    auto object = Napi::NewInstance(tpl);

    auto wrapper = new TargetWrapper { llvmTarget };
    wrapper->Wrap(object);

    return escapeScope.Escape(object);
}
