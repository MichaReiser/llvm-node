//
// Created by Micha Reiser on 02.03.17.
//

#ifndef LLVM_NODE_BITCODE_WRITER_H
#define LLVM_NODE_BITCODE_WRITER_H

#include <nan.h>
#include <llvm/Bitcode/BitcodeWriter.h>

NAN_MODULE_INIT(InitBitcodeWriter);

NAN_METHOD(WriteBitcodeToFile);

#endif //LLVM_NODE_BITCODE_WRITER_H
