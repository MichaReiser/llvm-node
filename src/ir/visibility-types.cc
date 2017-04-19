//
// Created by Micha Reiser on 11.04.17.
//

#include <llvm/IR/GlobalValue.h>
#include "visibility-types.h"

NAN_MODULE_INIT(InitVisibilityTypes) {
    auto visibilityTypes = Nan::New<v8::Object>();
    Nan::Set(visibilityTypes, Nan::New("Hidden").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::GlobalValue::VisibilityTypes::HiddenVisibility)));
    Nan::Set(visibilityTypes, Nan::New("Default").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::GlobalValue::VisibilityTypes::DefaultVisibility)));
    Nan::Set(visibilityTypes, Nan::New("Protected").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::GlobalValue::VisibilityTypes::ProtectedVisibility)));

    Nan::Set(target, Nan::New("VisibilityTypes").ToLocalChecked(), visibilityTypes);
}