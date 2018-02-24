#include <napi.h>

//#include "bitcode/bitcode.h"
#include "ir/ir.h"
//#include "support/support.h"
//#include "target/target.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
//    InitBitCode(env, exports);
    InitIR(env, exports );
//    InitSupport(target);
//    InitTarget(target);

    return exports;
}

NODE_API_MODULE(llvm, InitAll)