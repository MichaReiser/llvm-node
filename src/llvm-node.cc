#include <nan.h>

#include "bitcode/bitcode.h"
#include "ir/ir.h"
#include "config/config.h"
#include "support/support.h"
#include "target/target.h"
#include "jit/jit.h"

NAN_MODULE_INIT(InitAll) {
    InitBitCode(target);
    InitConfig(target);
    InitIR(target);
    InitJit(target);
    InitSupport(target);
    InitTarget(target);
}

NODE_MODULE(llvm, InitAll)