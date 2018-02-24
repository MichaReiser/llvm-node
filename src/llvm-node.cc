#include <napi.h>
#include <uv.h>

#include "bitcode/bitcode.h"
#include "ir/ir.h"
#include "config/config.h"
#include "support/support.h"
#include "target/target.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    InitBitCode(target);
    InitConfig(target);
    InitIR(target);
    InitSupport(target);
    InitTarget(target);
}

NODE_API_MODULE(llvm, InitAll)