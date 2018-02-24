//
// Created by Micha Reiser on 02.03.17.
//

#include <llvm/Support/raw_ostream.h>
#include "verifier.h"
#include "function.h"
#include "module.h"

Napi::Object InitVerifier(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "verifyFunction"), Napi::Function::New(env, verifyFunction));
    exports.Set(Napi::String::New(env, "verifyModule"), Napi::Function::New(env, verifyModule));
}

Napi::Value verifyFunction(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !FunctionWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "verifyFunction needs to be called with: fun: Function").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* function = FunctionWrapper::FromValue(info[0])->getFunction();
    static std::string msgPrefix { "Function verification failed: " };
    llvm::raw_string_ostream output { msgPrefix };

    if (llvm::verifyFunction(*function, &output)) {
        Napi::Error::New(env, output.str().c_str()).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::New(env, false);
}

Napi::Value verifyModule(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !ModuleWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "verifyModule needs to be called with: mod: Module").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* module = ModuleWrapper::FromValue(info[0])->getModule();
    static std::string msgPrefix { "Module verification failed: " };
    llvm::raw_string_ostream output { msgPrefix };

    if (llvm::verifyModule(*module, &output)) {
        Napi::Error::New(env, output.str().c_str()).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::New(env, false);
}