//
// Created by Micha Reiser on 02.03.17.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include "bitcode-writer.h"
#include "../ir/module.h"
#include "../util/string.h"

NAN_MODULE_INIT(InitBitcodeWriter) {
    Nan::SetMethod(target, "writeBitcodeToFile", WriteBitcodeToFile);
}

NAN_METHOD(WriteBitcodeToFile) {
    if (info.Length() != 2 || !ModuleWrapper::isInstance(info[0]) || !info[1]->IsString()) {
        return Nan::ThrowTypeError("writeBitcodeToFile needs to be called with: module: Module, filename: string");
    }

    auto* module = ModuleWrapper::FromValue(info[0])->getModule();
    auto fileName = ToString(Nan::To<v8::String>(info[1]).ToLocalChecked());

    std::error_code errorCode {};
    llvm::raw_fd_ostream byteCodeFile { fileName, errorCode, llvm::sys::fs::F_None };

    if (errorCode) {
        std::string messagePrefix { "Failed to open file: " };
        return Nan::ThrowError((messagePrefix + errorCode.message()).c_str());
    }

#if LLVM_VERSION_MAJOR > 6
    llvm::WriteBitcodeToFile(*module, byteCodeFile);
#else
    llvm::WriteBitcodeToFile(module, byteCodeFile);
#endif

    byteCodeFile.flush();
    byteCodeFile.close();
}