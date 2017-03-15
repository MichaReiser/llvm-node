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
#include "alloca-inst.h"
#include "pointer-type.h"
#include "array-type.h"
#include "calling-conv.h"
#include "call-inst.h"
#include "struct-type.h"

NAN_MODULE_INIT(InitIR) {
    AllocaInstWrapper::Init(target);
    ArgumentWrapper::Init(target);
    ArrayTypeWrapper::Init(target);
    BasicBlockWrapper::Init(target);
    InitCallingConv(target);
    CallInstWrapper::Init(target);
    ConstantWrapper::Init(target);
    ConstantFPWrapper::Init(target);
    ConstantIntWrapper::Init(target);
    ConstantPointerNullWrapper::Init(target);
    DataLayoutWrapper::Init(target);
    FunctionWrapper::Init(target);
    FunctionTypeWrapper::Init(target);
    IRBuilderWrapper::Init(target);
    InitLinkageTypes(target);
    ModuleWrapper::Init(target);
    LLVMContextWrapper::Init(target);
    PhiNodeWrapper::Init(target);
    PointerTypeWrapper::Init(target);
    StructTypeWrapper::Init(target);
    TypeWrapper::Init(target);
    ValueWrapper::Init(target);
    InitVerifier(target);
}