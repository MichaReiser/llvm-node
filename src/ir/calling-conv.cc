//
// Created by Micha Reiser on 13.03.17.
//

#include "calling-conv.h"
#include <llvm/Config/llvm-config.h>
#include <llvm/IR/CallingConv.h>

Napi::Object InitCallingConv(Napi::Env env, Napi::Object exports) {
    auto object = Napi::Object::New(env);

    (object).Set(Napi::String::New(env, "C"), Napi::New(env, llvm::CallingConv::C));
    (object).Set(Napi::String::New(env, "Fast"), Napi::New(env, llvm::CallingConv::Fast));
    (object).Set(Napi::String::New(env, "Cold"), Napi::New(env, llvm::CallingConv::Cold));
    (object).Set(Napi::String::New(env, "GHC"), Napi::New(env, llvm::CallingConv::GHC));
    (object).Set(Napi::String::New(env, "HiPE"), Napi::New(env, llvm::CallingConv::HiPE));
    (object).Set(Napi::String::New(env, "WebKit_JS"), Napi::New(env, llvm::CallingConv::WebKit_JS));
    (object).Set(Napi::String::New(env, "AnyReg"), Napi::New(env, llvm::CallingConv::AnyReg));
    (object).Set(Napi::String::New(env, "PreserveMost"), Napi::New(env, llvm::CallingConv::PreserveMost));
    (object).Set(Napi::String::New(env, "PreserveAll"), Napi::New(env, llvm::CallingConv::PreserveAll));
    (object).Set(Napi::String::New(env, "Swift"), Napi::New(env, llvm::CallingConv::Swift));
    (object).Set(Napi::String::New(env, "CXX_FAST_TLS"), Napi::New(env, llvm::CallingConv::CXX_FAST_TLS));
    (object).Set(Napi::String::New(env, "FirstTargetCC"), Napi::New(env, llvm::CallingConv::FirstTargetCC));
    (object).Set(Napi::String::New(env, "X86_StdCall"), Napi::New(env, llvm::CallingConv::X86_StdCall));
    (object).Set(Napi::String::New(env, "X86_FastCall"), Napi::New(env, llvm::CallingConv::X86_FastCall));
    (object).Set(Napi::String::New(env, "ARM_APCS"), Napi::New(env, llvm::CallingConv::ARM_APCS));
    (object).Set(Napi::String::New(env, "ARM_AAPCS"), Napi::New(env, llvm::CallingConv::ARM_AAPCS));
    (object).Set(Napi::String::New(env, "ARM_AAPCS_VFP"), Napi::New(env, llvm::CallingConv::ARM_AAPCS_VFP));
    (object).Set(Napi::String::New(env, "MSP430_INTR"), Napi::New(env, llvm::CallingConv::MSP430_INTR));
    (object).Set(Napi::String::New(env, "X86_ThisCall"), Napi::New(env, llvm::CallingConv::X86_ThisCall));
    (object).Set(Napi::String::New(env, "PTX_Kernel"), Napi::New(env, llvm::CallingConv::PTX_Kernel));
    (object).Set(Napi::String::New(env, "PTX_Device"), Napi::New(env, llvm::CallingConv::PTX_Device));
    (object).Set(Napi::String::New(env, "SPIR_FUNC"), Napi::New(env, llvm::CallingConv::SPIR_FUNC));
    (object).Set(Napi::String::New(env, "SPIR_KERNEL"), Napi::New(env, llvm::CallingConv::SPIR_KERNEL));
    (object).Set(Napi::String::New(env, "Intel_OCL_BI"), Napi::New(env, llvm::CallingConv::Intel_OCL_BI));
    (object).Set(Napi::String::New(env, "X86_64_SysV"), Napi::New(env, llvm::CallingConv::X86_64_SysV));

#if LLVM_VERSION_MAJOR == 4
    (object).Set(Napi::String::New(env, "X86_64_Win64"), Napi::New(env, llvm::CallingConv::X86_64_Win64));
#else
    (object).Set(Napi::String::New(env, "X86_64_Win64"), Napi::New(env, llvm::CallingConv::Win64));
    (object).Set(Napi::String::New(env, "Win64"), Napi::New(env, llvm::CallingConv::Win64));
#endif

    (object).Set(Napi::String::New(env, "X86_VectorCall"), Napi::New(env, llvm::CallingConv::X86_VectorCall));
    (object).Set(Napi::String::New(env, "HHVM"), Napi::New(env, llvm::CallingConv::HHVM));
    (object).Set(Napi::String::New(env, "HHVM_C"), Napi::New(env, llvm::CallingConv::HHVM_C));
    (object).Set(Napi::String::New(env, "X86_INTR"), Napi::New(env, llvm::CallingConv::X86_INTR));
    (object).Set(Napi::String::New(env, "AVR_INTR"), Napi::New(env, llvm::CallingConv::AVR_INTR));
    (object).Set(Napi::String::New(env, "AVR_SIGNAL"), Napi::New(env, llvm::CallingConv::AVR_SIGNAL));
    (object).Set(Napi::String::New(env, "AVR_BUILTIN"), Napi::New(env, llvm::CallingConv::AVR_BUILTIN));
    (object).Set(Napi::String::New(env, "AMDGPU_VS"), Napi::New(env, llvm::CallingConv::AMDGPU_VS));
    (object).Set(Napi::String::New(env, "AMDGPU_GS"), Napi::New(env, llvm::CallingConv::AMDGPU_GS));
    (object).Set(Napi::String::New(env, "AMDGPU_PS"), Napi::New(env, llvm::CallingConv::AMDGPU_PS));
    (object).Set(Napi::String::New(env, "AMDGPU_CS"), Napi::New(env, llvm::CallingConv::AMDGPU_CS));
    (object).Set(Napi::String::New(env, "AMDGPU_KERNEL"), Napi::New(env, llvm::CallingConv::AMDGPU_CS));
    (object).Set(Napi::String::New(env, "X86_RegCall"), Napi::New(env, llvm::CallingConv::AMDGPU_CS));
    (object).Set(Napi::String::New(env, "MSP430_BUILTIN"), Napi::New(env, llvm::CallingConv::AMDGPU_CS));
    (object).Set(Napi::String::New(env, "MaxID"), Napi::New(env, llvm::CallingConv::MaxID));

    (target).Set(Napi::String::New(env, "CallingConv"), object);
}