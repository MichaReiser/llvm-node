//
// Created by Micha Reiser on 02.03.17.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include "bitcode-writer.h"
#include "../ir/module.h"

void WriteBitcodeToFile(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    if (info.Length() != 2 || !ModuleWrapper::isInstanceOfType(info[0]) || !info[1].IsString()) {
        throw Napi::TypeError::New(env, "writeBitcodeToFile needs to be called with: module: Module, filename: string");
    }

    auto* module = ModuleWrapper::Unwrap(info[0].As<Napi::Object>())->getModule();
    std::string fileName = info[1].As<Napi::String>();

    std::error_code errorCode {};
    llvm::raw_fd_ostream byteCodeFile { fileName, errorCode, llvm::sys::fs::F_None };

    if (errorCode) {
        std::string messagePrefix { "Failed to open file: " };
        throw Napi::TypeError::New(env, (messagePrefix + errorCode.message()));
    }

#if LLVM_VERSION_MAJOR > 6
    llvm::WriteBitcodeToFile(*module, byteCodeFile);
#else
    llvm::WriteBitcodeToFile(module, byteCodeFile);
#endif

    byteCodeFile.flush();
    byteCodeFile.close();
}

void InitBitcodeWriter(Napi::Env env, Napi::Object& exports) {
    exports.Set("writeBitcodeToFile", Napi::Function::New(env, WriteBitcodeToFile));
}