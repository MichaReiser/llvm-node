//
// Created by Micha Reiser on 11.04.17.
//

#include <llvm/IR/GlobalValue.h>
#include "visibility-types.h"

Napi::Object InitVisibilityTypes(Napi::Env env, Napi::Object exports) {
    auto visibilityTypes = Napi::Object::New(env);
    (visibilityTypes).Set(Napi::String::New(env, "Hidden"), Napi::New(env, static_cast<uint32_t>(llvm::GlobalValue::VisibilityTypes::HiddenVisibility)));
    (visibilityTypes).Set(Napi::String::New(env, "Default"), Napi::New(env, static_cast<uint32_t>(llvm::GlobalValue::VisibilityTypes::DefaultVisibility)));
    (visibilityTypes).Set(Napi::String::New(env, "Protected"), Napi::New(env, static_cast<uint32_t>(llvm::GlobalValue::VisibilityTypes::ProtectedVisibility)));

    (target).Set(Napi::String::New(env, "VisibilityTypes"), visibilityTypes);
}