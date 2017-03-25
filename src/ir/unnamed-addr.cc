#include "unnamed-addr.h"
#include <llvm/IR/GlobalValue.h>

NAN_MODULE_INIT(InitUnnamedAddr) {
    auto unnamedAddr = Nan::New<v8::Object>();
    Nan::Set(unnamedAddr, Nan::New("None").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::GlobalValue::UnnamedAddr::None)));
    Nan::Set(unnamedAddr, Nan::New("Local").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::GlobalValue::UnnamedAddr::Local)));
    Nan::Set(unnamedAddr, Nan::New("Global").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::GlobalValue::UnnamedAddr::Global)));

    Nan::Set(target, Nan::New("UnnamedAddr").ToLocalChecked(), unnamedAddr);
}