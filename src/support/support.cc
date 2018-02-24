#include <llvm/Support/TargetSelect.h>
#include "support.h"

Napi::Value InitializeAllTargetInfos(const Napi::CallbackInfo& info) {
    llvm::InitializeAllTargetInfos();
}

Napi::Value InitializeAllTargets(const Napi::CallbackInfo& info) {
    llvm::InitializeAllTargets();
}

Napi::Value InitializeAllTargetMCs(const Napi::CallbackInfo& info) {
    llvm::InitializeAllTargetMCs();
}

Napi::Value InitializeAllAsmParsers(const Napi::CallbackInfo& info) {
    llvm::InitializeAllAsmParsers();
}

Napi::Value InitializeAllAsmPrinters(const Napi::CallbackInfo& info) {
    llvm::InitializeAllAsmPrinters();
}

Napi::Object InitSupport(Napi::Env env, Napi::Object exports) {
    (target).Set(Napi::String::New(env, "initializeAllTargetInfos"),
        Napi::GetFunction(Napi::Function::New(env, InitializeAllTargetInfos))
    );

    (target).Set(Napi::String::New(env, "initializeAllTargets"),
        Napi::GetFunction(Napi::Function::New(env, InitializeAllTargets))
    );

     (target).Set(Napi::String::New(env, "initializeAllTargetMCs"),
        Napi::GetFunction(Napi::Function::New(env, InitializeAllTargetMCs))
    );

     (target).Set(Napi::String::New(env, "initializeAllAsmParsers"),
        Napi::GetFunction(Napi::Function::New(env, InitializeAllAsmParsers))
    );

     (target).Set(Napi::String::New(env, "initializeAllAsmPrinters"),
        Napi::GetFunction(Napi::Function::New(env, InitializeAllAsmPrinters))
    );

    TargetRegistryWrapper::Init(env, target, module);
    TargetWrapper::Init(env, target, module);
}