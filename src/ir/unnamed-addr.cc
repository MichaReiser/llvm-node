#include "unnamed-addr.h"
#include <llvm/IR/GlobalValue.h>

Napi::Object InitUnnamedAddr(Napi::Env env, Napi::Object exports) {
    auto unnamedAddr = Napi::Object::New(env);
    (unnamedAddr).Set(Napi::String::New(env, "None"), Napi::New(env, static_cast<uint32_t>(llvm::GlobalValue::UnnamedAddr::None)));
    (unnamedAddr).Set(Napi::String::New(env, "Local"), Napi::New(env, static_cast<uint32_t>(llvm::GlobalValue::UnnamedAddr::Local)));
    (unnamedAddr).Set(Napi::String::New(env, "Global"), Napi::New(env, static_cast<uint32_t>(llvm::GlobalValue::UnnamedAddr::Global)));

    (target).Set(Napi::String::New(env, "UnnamedAddr"), unnamedAddr);
}