//
// Created by Micha Reiser on 02.03.17.
//

#include "config.h"
#include "llvm-config.h"

NAN_MODULE_INIT(InitConfig) {
    InitLLVMConfig(target);
}