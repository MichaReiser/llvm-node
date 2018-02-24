//
// Created by Micha Reiser on 02.03.17.
//

#ifndef LLVM_NODE_CONFIG_H
#define LLVM_NODE_CONFIG_H

#include <napi.h>
#include <uv.h>

Napi::Object InitConfig(Napi::Env env, Napi::Object exports);

#endif //LLVM_NODE_CONFIG_H
