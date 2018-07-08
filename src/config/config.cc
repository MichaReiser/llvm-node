//
// Created by Micha Reiser on 02.03.17.
//

#include "config.h"
#include "llvm-config.h"

void InitConfig(Napi::Env env, Napi::Object& exports) {
    InitLLVMConfig(env, exports);
}