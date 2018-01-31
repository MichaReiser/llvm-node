//
// Created by Micha Reiser on 02.03.17.
//

#include <llvm/Config/llvm-config.h>
#include "llvm-config.h"

NAN_MODULE_INIT(InitLLVMConfig) {
    auto config = Nan::New<v8::Object>();

    Nan::Set(config, Nan::New("LLVM_VERSION_MAJOR").ToLocalChecked(), Nan::New(static_cast<uint32_t>(LLVM_VERSION_MAJOR)));
    Nan::Set(config, Nan::New("LLVM_VERSION_MINOR").ToLocalChecked(), Nan::New(static_cast<uint32_t>(LLVM_VERSION_MINOR)));
    Nan::Set(config, Nan::New("LLVM_VERSION_PATCH").ToLocalChecked(), Nan::New(static_cast<uint32_t>(LLVM_VERSION_PATCH)));
    Nan::Set(config, Nan::New("LLVM_VERSION_STRING").ToLocalChecked(), Nan::New(LLVM_VERSION_STRING).ToLocalChecked());
    Nan::Set(config, Nan::New("LLVM_DEFAULT_TARGET_TRIPLE").ToLocalChecked(), Nan::New(LLVM_DEFAULT_TARGET_TRIPLE).ToLocalChecked());

    Nan::Set(target, Nan::New("config").ToLocalChecked(), config);
}