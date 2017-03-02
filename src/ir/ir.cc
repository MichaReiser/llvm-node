//
// Created by Micha Reiser on 01.03.17.
//

#include "ir.h"
#include "data-layout.h"
#include "function-type.h"
#include "ir-builder.h"
#include "module.h"
#include "llvm-context.h"
#include "type.h"
#include "value.h"
#include "function.h"
#include "linkage-types.h"
#include "argument.h"
#include "basic-block.h"
#include "phi-node.h"
#include "verifier.h"

NAN_MODULE_INIT(InitIR) {
    ArgumentWrapper::Init(target);
    BasicBlockWrapper::Init(target);
    ConstantWrapper::Init(target);
    ConstantFPWrapper::Init(target);
    DataLayoutWrapper::Init(target);
    FunctionWrapper::Init(target);
    FunctionTypeWrapper::Init(target);
    IRBuilderWrapper::Init(target);
    InitLinkageTypes(target);
    ModuleWrapper::Init(target);
    LLVMContextWrapper::Init(target);
    PhiNodeWrapper::Init(target);
    TypeWrapper::Init(target);
    ValueWrapper::Init(target);
    InitVerifier(target);
}