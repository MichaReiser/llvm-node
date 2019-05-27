#include <nan.h>

#include "bitcode/bitcode.h"
#include "ir/ir.h"
#include "config/config.h"
#include "support/support.h"
#include "target/target.h"
#include "dwarf/dwarf.h"

NAN_MODULE_INIT(InitAll) {
    InitBitCode(target);
    InitConfig(target);
    InitDwarf(target);
    InitIR(target);
    InitSupport(target);
    InitTarget(target);
}

NODE_MODULE(llvm, InitAll)