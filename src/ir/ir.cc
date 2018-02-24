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
#include "constant-data-array.h"
#include "constant-fp.h"
#include "constant-int.h"
#include "constant-pointer-null.h"
#include "global-variable.h"
#include "unnamed-addr.h"
#include "constant-struct.h"
#include "constant-array.h"
#include "visibility-types.h"
#include "attribute.h"
#include "undef-value.h"

Napi::Object InitIR(Napi::Env env, Napi::Object exports) {
    AllocaInstWrapper::Init(env, target, module);
    ArgumentWrapper::Init(env, target, module);
    ArrayTypeWrapper::Init(env, target, module);
    InitAttribute(target);
    BasicBlockWrapper::Init(env, target, module);
    ConstantStructWrapper::Init(env, target, module);
    InitCallingConv(target);
    InitUnnamedAddr(target);
    CallInstWrapper::Init(env, target, module);
    ConstantWrapper::Init(env, target, module);
    ConstantArrayWrapper::Init(env, target, module);
    ConstantDataArrayWrapper::Init(env, target, module);
    ConstantFPWrapper::Init(env, target, module);
    ConstantIntWrapper::Init(env, target, module);
    ConstantPointerNullWrapper::Init(env, target, module);
    DataLayoutWrapper::Init(env, target, module);
    FunctionWrapper::Init(env, target, module);
    FunctionTypeWrapper::Init(env, target, module);
    GlobalVariableWrapper::Init(env, target, module);
    IRBuilderWrapper::Init(env, target, module);
    InitLinkageTypes(target);
    ModuleWrapper::Init(env, target, module);
    LLVMContextWrapper::Init(env, target, module);
    PhiNodeWrapper::Init(env, target, module);
    PointerTypeWrapper::Init(env, target, module);
    StructTypeWrapper::Init(env, target, module);
    TypeWrapper::Init(env, target, module);
    UndefValueWrapper::Init(env, target, module);
    ValueWrapper::Init(env, target, module);
    InitVerifier(target);
    InitVisibilityTypes(target);
}