//
// Created by Micha Reiser on 02.03.17.
//

#ifndef LLVM_NODE_BITCODE_WRITER_H
#define LLVM_NODE_BITCODE_WRITER_H

#include <napi.h>
#include <llvm/Bitcode/BitcodeWriter.h>

void InitBitcodeWriter(Napi::Env env, Napi::Object exports);

#endif //LLVM_NODE_BITCODE_WRITER_H
