//
// Created by Micha Reiser on 13.03.17.
//

#ifndef LLVM_NODE_CALLING_CONV_H
#define LLVM_NODE_CALLING_CONV_H

#include <napi.h>
#include <uv.h>

Napi::Object InitCallingConv(Napi::Env env, Napi::Object exports);

#endif //LLVM_NODE_CALLING_CONV_H
