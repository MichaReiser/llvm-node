//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_LINKAGE_TYPES_H
#define LLVM_NODE_LINKAGE_TYPES_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/GlobalValue.h>

Napi::Object InitLinkageTypes(Napi::Env env, Napi::Object exports);

#endif //LLVM_NODE_LINKAGE_TYPES_H
