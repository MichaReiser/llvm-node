//
// Created by Micha Reiser on 01.03.17.
//

#include "linkage-types.h"

Napi::Object InitLinkageTypes(Napi::Env env, Napi::Object exports) {
    auto object = Napi::Object::New(env);

    (object).Set(Napi::String::New(env, "ExternalLinkage"), Napi::New(env, llvm::GlobalValue::LinkageTypes::ExternalLinkage));
    (object).Set(Napi::String::New(env, "AvailableExternallyLinkage"), Napi::New(env, llvm::GlobalValue::LinkageTypes::AvailableExternallyLinkage));
    (object).Set(Napi::String::New(env, "LinkOnceAnyLinkage"), Napi::New(env, llvm::GlobalValue::LinkageTypes::LinkOnceAnyLinkage));
    (object).Set(Napi::String::New(env, "LinkOnceODRLinkage"), Napi::New(env, llvm::GlobalValue::LinkageTypes::LinkOnceODRLinkage));
    (object).Set(Napi::String::New(env, "WeakAnyLinkage"), Napi::New(env, llvm::GlobalValue::LinkageTypes::WeakAnyLinkage));
    (object).Set(Napi::String::New(env, "WeakODRLinkage"), Napi::New(env, llvm::GlobalValue::LinkageTypes::WeakODRLinkage));
    (object).Set(Napi::String::New(env, "AppendingLinkage"), Napi::New(env, llvm::GlobalValue::LinkageTypes::AppendingLinkage));
    (object).Set(Napi::String::New(env, "InternalLinkage"), Napi::New(env, llvm::GlobalValue::LinkageTypes::InternalLinkage));
    (object).Set(Napi::String::New(env, "PrivateLinkage"), Napi::New(env, llvm::GlobalValue::LinkageTypes::PrivateLinkage));
    (object).Set(Napi::String::New(env, "ExternalWeakLinkage"), Napi::New(env, llvm::GlobalValue::LinkageTypes::ExternalWeakLinkage));
    (object).Set(Napi::String::New(env, "CommonLinkage"), Napi::New(env, llvm::GlobalValue::LinkageTypes::CommonLinkage));

    (target).Set(Napi::String::New(env, "LinkageTypes"), object);
}