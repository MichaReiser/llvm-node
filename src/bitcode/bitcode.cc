//
// Created by Micha Reiser on 02.03.17.
//

#include "bitcode.h"
#include "bitcode-writer.h"

NAN_MODULE_INIT(InitBitCode) {
    InitBitcodeWriter(target);
}