#include <napi.h>

//#include "bitcode/bitcode.h"
//#include "ir/ir.h"
#include "config/config.h"
#include "support/support.h"
#include "target/target.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
//    InitBitCode(target);
    InitConfig(env, exports);
//    InitIR(target);
    InitSupport(env, exports);
    InitTarget(env, exports);

    return exports;
}

NODE_API_MODULE(hello, Init);