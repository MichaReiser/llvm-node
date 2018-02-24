//
// Created by Micha Reiser on 02.03.17.
//

#include <llvm/Config/llvm-config.h>
#include "llvm-config.h"

Napi::Object InitLLVMConfig(Napi::Env env, Napi::Object exports) {
    auto config = Napi::Object::New(env);

    (config).Set(Napi::String::New(env, "LLVM_VERSION_MAJOR"), Napi::New(env, static_cast<uint32_t>(LLVM_VERSION_MAJOR)));
    (config).Set(Napi::String::New(env, "LLVM_VERSION_MINOR"), Napi::New(env, static_cast<uint32_t>(LLVM_VERSION_MINOR)));
    (config).Set(Napi::String::New(env, "LLVM_VERSION_PATCH"), Napi::New(env, static_cast<uint32_t>(LLVM_VERSION_PATCH)));
    (config).Set(Napi::String::New(env, "LLVM_VERSION_STRING"), Napi::New(env, LLVM_VERSION_STRING));
    (config).Set(Napi::String::New(env, "LLVM_DEFAULT_TARGET_TRIPLE"), Napi::New(env, LLVM_DEFAULT_TARGET_TRIPLE));

    (target).Set(Napi::String::New(env, "config"), config);
}