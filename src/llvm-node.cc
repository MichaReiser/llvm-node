#include <nan.h>

#include "bitcode/bitcode.h"
#include "ir/ir.h"
#include "config/config.h"
#include "support/support.h"
#include "target/target.h"
#include "execution-engine/execution-engine-module.h"

NAN_MODULE_INIT(InitAll) {
    InitBitCode(target);
    InitConfig(target);
    InitIR(target);
    InitSupport(target);
    InitExecutionEngine(target);
}

NODE_MODULE(llvm, InitAll)