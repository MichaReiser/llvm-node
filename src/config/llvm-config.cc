//
// Created by Micha Reiser on 02.03.17.
//

#include <llvm/Config/llvm-config.h>
#include "llvm-config.h"

void InitLLVMConfig(Napi::Env env, Napi::Object& exports) {
    Napi::EscapableHandleScope escapeScope { env };
    Napi::Object config = Napi::Object::New(env);

    config.Set("LLVM_VERSION_MAJOR", Napi::Number::New(env, static_cast<uint32_t>(LLVM_VERSION_MAJOR)));
    config.Set("LLVM_VERSION_MINOR", Napi::Number::New(env, static_cast<uint32_t>(LLVM_VERSION_MINOR)));
    config.Set("LLVM_VERSION_PATCH", Napi::Number::New(env, static_cast<uint32_t>(LLVM_VERSION_PATCH)));
    config.Set("LLVM_VERSION_STRING", Napi::String::New(env, LLVM_VERSION_STRING));
    config.Set("LLVM_DEFAULT_TARGET_TRIPLE", Napi::String::New(env, LLVM_DEFAULT_TARGET_TRIPLE));

    exports.Set("config", escapeScope.Escape(config));
}