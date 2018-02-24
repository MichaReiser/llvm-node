//
// Created by Micha Reiser on 02.03.17.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include "bitcode-writer.h"
#include "../ir/module.h"
#include "../util/string.h"

Napi::Object InitBitcodeWriter(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "writeBitcodeToFile"), Napi::Function::New(env, WriteBitcodeToFile));
}

Napi::Value WriteBitcodeToFile(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !ModuleWrapper::isInstance(info[0]) || !info[1].IsString()) {
        Napi::TypeError::New(env, "writeBitcodeToFile needs to be called with: module: Module, filename: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* module = ModuleWrapper::FromValue(info[0])->getModule();
    auto fileName = ToString(info[1].To<Napi::String>());

    std::error_code errorCode {};
    llvm::raw_fd_ostream byteCodeFile { fileName, errorCode, llvm::sys::fs::F_None };

    if (errorCode) {
        std::string messagePrefix { "Failed to open file: " };
        Napi::Error::New(env, (messagePrefix + errorCode.message()).c_str()).ThrowAsJavaScriptException();
        return env.Null();
    }

#if LLVM_VERSION_MAJOR > 6
    llvm::WriteBitcodeToFile(*module, byteCodeFile);
#else
    llvm::WriteBitcodeToFile(module, byteCodeFile);
#endif

    byteCodeFile.flush();
    byteCodeFile.close();
}