//
// Created by Micha Reiser on 13.03.17.
//

#include "calling-conv.h"
#include <llvm/Config/llvm-config.h>
#include <llvm/IR/CallingConv.h>

NAN_MODULE_INIT(InitCallingConv) {
    auto object = Nan::New<v8::Object>();

    Nan::Set(object, Nan::New("C").ToLocalChecked(), Nan::New(llvm::CallingConv::C));
    Nan::Set(object, Nan::New("Fast").ToLocalChecked(), Nan::New(llvm::CallingConv::Fast));
    Nan::Set(object, Nan::New("Cold").ToLocalChecked(), Nan::New(llvm::CallingConv::Cold));
    Nan::Set(object, Nan::New("GHC").ToLocalChecked(), Nan::New(llvm::CallingConv::GHC));
    Nan::Set(object, Nan::New("HiPE").ToLocalChecked(), Nan::New(llvm::CallingConv::HiPE));
    Nan::Set(object, Nan::New("WebKit_JS").ToLocalChecked(), Nan::New(llvm::CallingConv::WebKit_JS));
    Nan::Set(object, Nan::New("AnyReg").ToLocalChecked(), Nan::New(llvm::CallingConv::AnyReg));
    Nan::Set(object, Nan::New("PreserveMost").ToLocalChecked(), Nan::New(llvm::CallingConv::PreserveMost));
    Nan::Set(object, Nan::New("PreserveAll").ToLocalChecked(), Nan::New(llvm::CallingConv::PreserveAll));
    Nan::Set(object, Nan::New("Swift").ToLocalChecked(), Nan::New(llvm::CallingConv::Swift));
    Nan::Set(object, Nan::New("CXX_FAST_TLS").ToLocalChecked(), Nan::New(llvm::CallingConv::CXX_FAST_TLS));
    Nan::Set(object, Nan::New("FirstTargetCC").ToLocalChecked(), Nan::New(llvm::CallingConv::FirstTargetCC));
    Nan::Set(object, Nan::New("X86_StdCall").ToLocalChecked(), Nan::New(llvm::CallingConv::X86_StdCall));
    Nan::Set(object, Nan::New("X86_FastCall").ToLocalChecked(), Nan::New(llvm::CallingConv::X86_FastCall));
    Nan::Set(object, Nan::New("ARM_APCS").ToLocalChecked(), Nan::New(llvm::CallingConv::ARM_APCS));
    Nan::Set(object, Nan::New("ARM_AAPCS").ToLocalChecked(), Nan::New(llvm::CallingConv::ARM_AAPCS));
    Nan::Set(object, Nan::New("ARM_AAPCS_VFP").ToLocalChecked(), Nan::New(llvm::CallingConv::ARM_AAPCS_VFP));
    Nan::Set(object, Nan::New("MSP430_INTR").ToLocalChecked(), Nan::New(llvm::CallingConv::MSP430_INTR));
    Nan::Set(object, Nan::New("X86_ThisCall").ToLocalChecked(), Nan::New(llvm::CallingConv::X86_ThisCall));
    Nan::Set(object, Nan::New("PTX_Kernel").ToLocalChecked(), Nan::New(llvm::CallingConv::PTX_Kernel));
    Nan::Set(object, Nan::New("PTX_Device").ToLocalChecked(), Nan::New(llvm::CallingConv::PTX_Device));
    Nan::Set(object, Nan::New("SPIR_FUNC").ToLocalChecked(), Nan::New(llvm::CallingConv::SPIR_FUNC));
    Nan::Set(object, Nan::New("SPIR_KERNEL").ToLocalChecked(), Nan::New(llvm::CallingConv::SPIR_KERNEL));
    Nan::Set(object, Nan::New("Intel_OCL_BI").ToLocalChecked(), Nan::New(llvm::CallingConv::Intel_OCL_BI));
    Nan::Set(object, Nan::New("X86_64_SysV").ToLocalChecked(), Nan::New(llvm::CallingConv::X86_64_SysV));

#if LLVM_VERSION_MAJOR == 4
    Nan::Set(object, Nan::New("X86_64_Win64").ToLocalChecked(), Nan::New(llvm::CallingConv::X86_64_Win64));
#else
    Nan::Set(object, Nan::New("X86_64_Win64").ToLocalChecked(), Nan::New(llvm::CallingConv::Win64));
    Nan::Set(object, Nan::New("Win64").ToLocalChecked(), Nan::New(llvm::CallingConv::Win64));
#endif

    Nan::Set(object, Nan::New("X86_VectorCall").ToLocalChecked(), Nan::New(llvm::CallingConv::X86_VectorCall));
    Nan::Set(object, Nan::New("HHVM").ToLocalChecked(), Nan::New(llvm::CallingConv::HHVM));
    Nan::Set(object, Nan::New("HHVM_C").ToLocalChecked(), Nan::New(llvm::CallingConv::HHVM_C));
    Nan::Set(object, Nan::New("X86_INTR").ToLocalChecked(), Nan::New(llvm::CallingConv::X86_INTR));
    Nan::Set(object, Nan::New("AVR_INTR").ToLocalChecked(), Nan::New(llvm::CallingConv::AVR_INTR));
    Nan::Set(object, Nan::New("AVR_SIGNAL").ToLocalChecked(), Nan::New(llvm::CallingConv::AVR_SIGNAL));
    Nan::Set(object, Nan::New("AVR_BUILTIN").ToLocalChecked(), Nan::New(llvm::CallingConv::AVR_BUILTIN));
    Nan::Set(object, Nan::New("AMDGPU_VS").ToLocalChecked(), Nan::New(llvm::CallingConv::AMDGPU_VS));
    Nan::Set(object, Nan::New("AMDGPU_GS").ToLocalChecked(), Nan::New(llvm::CallingConv::AMDGPU_GS));
    Nan::Set(object, Nan::New("AMDGPU_PS").ToLocalChecked(), Nan::New(llvm::CallingConv::AMDGPU_PS));
    Nan::Set(object, Nan::New("AMDGPU_CS").ToLocalChecked(), Nan::New(llvm::CallingConv::AMDGPU_CS));
    Nan::Set(object, Nan::New("AMDGPU_KERNEL").ToLocalChecked(), Nan::New(llvm::CallingConv::AMDGPU_CS));
    Nan::Set(object, Nan::New("X86_RegCall").ToLocalChecked(), Nan::New(llvm::CallingConv::AMDGPU_CS));
    Nan::Set(object, Nan::New("MSP430_BUILTIN").ToLocalChecked(), Nan::New(llvm::CallingConv::AMDGPU_CS));
    Nan::Set(object, Nan::New("MaxID").ToLocalChecked(), Nan::New(llvm::CallingConv::MaxID));

    Nan::Set(target, Nan::New("CallingConv").ToLocalChecked(), object);
}