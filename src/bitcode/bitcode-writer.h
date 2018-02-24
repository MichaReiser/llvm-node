//
// Created by Micha Reiser on 02.03.17.
//

#ifndef LLVM_NODE_BITCODE_WRITER_H
#define LLVM_NODE_BITCODE_WRITER_H

#include <napi.h>
#include <uv.h>
#include <llvm/Bitcode/BitcodeWriter.h>

Napi::Object InitBitcodeWriter(Napi::Env env, Napi::Object exports);

Napi::Value WriteBitcodeToFile(const Napi::CallbackInfo& info);

#endif //LLVM_NODE_BITCODE_WRITER_H
