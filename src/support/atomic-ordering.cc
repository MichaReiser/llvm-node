#include <llvm/Support/AtomicOrdering.h>
#include "atomic-ordering.h"

NAN_MODULE_INIT(InitAtomicOrdering) {
    auto atomicOrdering = Nan::New<v8::Object>();
    Nan::Set(atomicOrdering, Nan::New("NotAtomic").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicOrdering::NotAtomic)));
    Nan::Set(atomicOrdering, Nan::New("Unordered").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicOrdering::Unordered)));
    Nan::Set(atomicOrdering, Nan::New("Monotonic").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicOrdering::Monotonic)));
    Nan::Set(atomicOrdering, Nan::New("Acquire").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicOrdering::Acquire)));
    Nan::Set(atomicOrdering, Nan::New("Release").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicOrdering::Release)));
    Nan::Set(atomicOrdering, Nan::New("AcquireRelease").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicOrdering::AcquireRelease)));
    Nan::Set(atomicOrdering, Nan::New("SequentiallyConsistent").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::AtomicOrdering::SequentiallyConsistent)));

    Nan::Set(target, Nan::New("AtomicOrdering").ToLocalChecked(), atomicOrdering);
}
