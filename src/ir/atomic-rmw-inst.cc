#include <llvm/IR/Instructions.h>
#include "atomic-rmw-inst.h"

NAN_MODULE_INIT(InitAtomicRMWInst) {
    auto atomicRMWInst = Nan::New<v8::Object>();

    auto binOp = Nan::New<v8::Object>();
    Nan::Set(binOp, Nan::New("Add").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicRMWInst::BinOp::Add)));
    Nan::Set(binOp, Nan::New("Sub").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicRMWInst::BinOp::Sub)));
    Nan::Set(binOp, Nan::New("And").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicRMWInst::BinOp::And)));
    Nan::Set(binOp, Nan::New("Nand").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicRMWInst::BinOp::Nand)));
    Nan::Set(binOp, Nan::New("Or").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicRMWInst::BinOp::Or)));
    Nan::Set(binOp, Nan::New("Xor").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicRMWInst::BinOp::Xor)));
    Nan::Set(binOp, Nan::New("Max").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicRMWInst::BinOp::Max)));
    Nan::Set(binOp, Nan::New("Min").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicRMWInst::BinOp::Min)));
    Nan::Set(binOp, Nan::New("UMax").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicRMWInst::BinOp::UMax)));
    Nan::Set(binOp, Nan::New("UMin").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicRMWInst::BinOp::UMin)));

    Nan::Set(atomicRMWInst, Nan::New("BinOp").ToLocalChecked(), binOp);

    Nan::Set(target, Nan::New("AtomicRMWInst").ToLocalChecked(), atomicRMWInst);
}
