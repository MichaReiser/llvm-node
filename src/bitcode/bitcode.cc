//
// Created by Micha Reiser on 02.03.17.
//

#include "bitcode.h"
#include "bitcode-writer.h"

Napi::Object InitBitCode(Napi::Env env, Napi::Object exports) {
    InitBitcodeWriter(target);
}