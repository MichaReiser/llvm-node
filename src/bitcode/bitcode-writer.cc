//
// Created by Micha Reiser on 02.03.17.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include "bitcode-writer.h"
#include "../ir/module.h"
//#include "../util/string.h"

Napi::Value WriteBitcodeToFile(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() != 2 || !ModuleWrapper::isInstanceOfType(info[0]) || !info[1].IsString()) {
        throw Napi::TypeError::New(env, "writeBitcodeToFile needs to be called with: module: Module, filename: string");
    }

    auto *module = ModuleWrapper::Unwrap(info[0].As<Napi::Object>())->getModule();
    const std::string fileName = info[1].As<Napi::String>();

    std::error_code errorCode{};
    llvm::raw_fd_ostream byteCodeFile{fileName, errorCode, llvm::sys::fs::F_None};

    if (errorCode) {
        const std::string message = "Failed to open file: " + errorCode.message();
        throw Napi::Error::New(env, Napi::String::New(env, message));
    }

    llvm::WriteBitcodeToFile(*module, byteCodeFile);

    byteCodeFile.flush();
    byteCodeFile.close();
}

void InitBitcodeWriter(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "writeBitcodeToFile"), Napi::Function::New(env, WriteBitcodeToFile));
}

