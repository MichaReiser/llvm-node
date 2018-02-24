//
// Created by Micha Reiser on 02.03.17.
//

#include "config.h"
#include "llvm-config.h"

Napi::Object InitConfig(Napi::Env env, Napi::Object exports) {
    InitLLVMConfig(target);
}