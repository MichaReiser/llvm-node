//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_IRH_H
#define LLVM_NODE_IRH_H

#include <napi.h>
#include <uv.h>

Napi::Object InitIR(Napi::Env env, Napi::Object exports);

#endif //LLVM_NODE_IRH_H
