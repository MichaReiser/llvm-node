//
// Created by Micha Reiser on 01.03.17.
//

#include "linkage-types.h"

NAN_MODULE_INIT(InitLinkageTypes) {
    auto object = Nan::New<v8::Object>();

    Nan::Set(object, Nan::New("ExternalLinkage").ToLocalChecked(), Nan::New(llvm::GlobalValue::LinkageTypes::ExternalLinkage));
    Nan::Set(object, Nan::New("AvailableExternallyLinkage").ToLocalChecked(), Nan::New(llvm::GlobalValue::LinkageTypes::AvailableExternallyLinkage));
    Nan::Set(object, Nan::New("LinkOnceAnyLinkage").ToLocalChecked(), Nan::New(llvm::GlobalValue::LinkageTypes::LinkOnceAnyLinkage));
    Nan::Set(object, Nan::New("LinkOnceODRLinkage").ToLocalChecked(), Nan::New(llvm::GlobalValue::LinkageTypes::LinkOnceODRLinkage));
    Nan::Set(object, Nan::New("WeakAnyLinkage").ToLocalChecked(), Nan::New(llvm::GlobalValue::LinkageTypes::WeakAnyLinkage));
    Nan::Set(object, Nan::New("WeakODRLinkage").ToLocalChecked(), Nan::New(llvm::GlobalValue::LinkageTypes::WeakODRLinkage));
    Nan::Set(object, Nan::New("AppendingLinkage").ToLocalChecked(), Nan::New(llvm::GlobalValue::LinkageTypes::AppendingLinkage));
    Nan::Set(object, Nan::New("InternalLinkage").ToLocalChecked(), Nan::New(llvm::GlobalValue::LinkageTypes::InternalLinkage));
    Nan::Set(object, Nan::New("PrivateLinkage").ToLocalChecked(), Nan::New(llvm::GlobalValue::LinkageTypes::PrivateLinkage));
    Nan::Set(object, Nan::New("ExternalWeakLinkage").ToLocalChecked(), Nan::New(llvm::GlobalValue::LinkageTypes::ExternalWeakLinkage));
    Nan::Set(object, Nan::New("CommonLinkage").ToLocalChecked(), Nan::New(llvm::GlobalValue::LinkageTypes::CommonLinkage));

    Nan::Set(target, Nan::New("LinkageTypes").ToLocalChecked(), object);
}