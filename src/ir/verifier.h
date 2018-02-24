//
// Created by Micha Reiser on 02.03.17.
//

#ifndef LLVM_NODE_VERIFIER_H
#define LLVM_NODE_VERIFIER_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Verifier.h>

Napi::Object InitVerifier(Napi::Env env, Napi::Object exports);

Napi::Value verifyModule(const Napi::CallbackInfo& info);
Napi::Value verifyFunction(const Napi::CallbackInfo& info);

#endif //LLVM_NODE_VERIFIER_H
