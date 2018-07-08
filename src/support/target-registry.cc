#include "../util/string.h"
#include "target-registry.h"
#include "../target/target-machine.h"

Napi::Value lookupTarget(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    if (info.Length() != 1 || !info[0].IsString()) {
        throw Napi::TypeError::New(env, "lookupTarget needs to be called with a single string argument");
    }

    std::string triple = info[0].As<Napi::String>();
    std::string error {};

    const llvm::Target* result = llvm::TargetRegistry::lookupTarget(triple, error);

    if (!result) {
        std::string msg = "Failed to lookup target: " + error;
        throw Napi::Error::New(env, msg);
    }

    return TargetWrapper::of(env, const_cast<llvm::Target*>(result));
}

void InitTargetRegistry(Napi::Env env, Napi::Object& exports) {
    Napi::Object targetRegistry = Napi::Object::New(env);
    targetRegistry.Set("lookupTarget", Napi::Function::New(env, &lookupTarget));

    exports.Set("TargetRegistry", targetRegistry);
}



//--------------------------------------------------------------
// TargetWrapper
//--------------------------------------------------------------

Napi::FunctionReference TargetWrapper::constructor;

void TargetWrapper::Init(Napi::Env env, Napi::Object &exports) {
    Napi::Function func = DefineClass(env, "Target", {
        InstanceMethod("createTargetMachine", &TargetWrapper::createTargetMachine),
        InstanceAccessor("name", &TargetWrapper::getName, nullptr),
        InstanceAccessor("shortDescription", &TargetWrapper::getShortDescription, nullptr)
    });

    constructor = Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Target", func);
}

Napi::Object TargetWrapper::of(Napi::Env env, llvm::Target *target) {
    return constructor.New({Napi::External<llvm::Target>::New(env, target)});
}

TargetWrapper::TargetWrapper(const Napi::CallbackInfo &info): Napi::ObjectWrap<TargetWrapper> { info } {
    auto env = info.Env();
    auto argsLength = info.Length();

    if (!info.IsConstructCall()) {
        throw Napi::TypeError::New(env, "Target constructor needs to be called with new");
    }

    if (!argsLength < 0 || !info[0].IsExternal()) {
        throw Napi::TypeError::New(env, "Target constructor needs to be called with a pointer to a target");
    }

    auto external = info[0].As<Napi::External<llvm::Target>>();
    this->target = external.Data();
}


Napi::Value TargetWrapper::createTargetMachine(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    if (info.Length() < 2 || !info[0].IsString() || !info[1].IsString()) {
        throw Napi::TypeError::New(env, "Function needs to be called at least with the arguments: triple: string, cpu: string.");
    }

    std::string targetTriple = info[0].As<Napi::String>();
    std::string cpu = info[1].As<Napi::String>();
    std::string features {};

    if (info.Length() > 2) {
        if (info[2].IsString()) {
            features = info[2].As<Napi::String>();
        } else {
            throw Napi::TypeError::New(env, "The 3th argument of createTargetMachine needs to be a string");
        }
    }

    llvm::TargetOptions options {};
    auto* targetMachinePtr = target->createTargetMachine(targetTriple, cpu, features, options, llvm::Optional<llvm::Reloc::Model> {});
    return TargetMachineWrapper::of(env, targetMachinePtr);
}

Napi::Value TargetWrapper::getName(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    return Napi::String::New(env, target->getName());
}

Napi::Value TargetWrapper::getShortDescription(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    return Napi::String::New(env, target->getShortDescription());
}