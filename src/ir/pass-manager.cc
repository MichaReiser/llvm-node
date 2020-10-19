//
// Created by lx.codev on 10.07.2020.
//

#include "pass-manager.h"

#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>

#include "../util/string.h"
#include "llvm-context.h"
#include "function.h"
#include "module.h"
#include "pass.h"

NAN_MODULE_INIT(PassManagerWrapper::Init) {
  auto constructorFunction =
      Nan::GetFunction(Nan::New(pmTemplate())).ToLocalChecked();
  Nan::Set(target, Nan::New("PassManager").ToLocalChecked(),
           constructorFunction);
}

NAN_METHOD(PassManagerWrapper::New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError(
        "Class Constructor PassManager cannot be invoked without new");
  }

  if (info.Length() != 1 || !info[0]->IsExternal()) {
    return Nan::ThrowTypeError("External PassManager Pointer required");
  }

  auto *wrapper = new PassManagerWrapper{};
  wrapper->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &PassManagerWrapper::pmTemplate() {
  static Nan::Persistent<v8::FunctionTemplate> tmpl{};

  if (tmpl.IsEmpty()) {
    v8::Local<v8::FunctionTemplate> localTemplate =
        Nan::New<v8::FunctionTemplate>(PassManagerWrapper::New);
    localTemplate->SetClassName(Nan::New("PassManager").ToLocalChecked());
    localTemplate->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(localTemplate, "add", PassManagerWrapper::add);
    Nan::SetPrototypeMethod(localTemplate, "run", PassManagerWrapper::run);

    tmpl.Reset(localTemplate);
  }

  return tmpl;
}

NAN_METHOD(PassManagerWrapper::add) {
  if (info.Length() != 1 || !PassWrapper::isInstance(info[0])) {
    return Nan::ThrowTypeError("addk needs to be called with: p: Pass");
  }

  auto *wrapper = PassManagerWrapper::FromValue(info.Holder());
  auto *pass = PassWrapper::FromValue(info[0])->getPass();
  wrapper->pm->add(pass);
}

NAN_METHOD(PassManagerWrapper::run) {
  if (info.Length() != 1 || !ModuleWrapper::isInstance(info[0])) {
    return Nan::ThrowTypeError("run needs to be called with: m: Module");
  }

  auto *wrapper = PassManagerWrapper::FromValue(info.Holder());
  auto *module = ModuleWrapper::FromValue(info[0])->getModule();
  bool result = wrapper->pm->run(*module);
  info.GetReturnValue().Set(Nan::New(result));
}

llvm::legacy::PassManager *PassManagerWrapper::getPassManager() { return pm; }

bool PassManagerWrapper::isInstance(v8::Local<v8::Value> value) {
  return Nan::New(pmTemplate())->HasInstance(value);
}

// FunctionPassManagerWrapper

NAN_MODULE_INIT(FunctionPassManagerWrapper::Init) {
  auto constructorFunction =
      Nan::GetFunction(Nan::New(fpmTemplate())).ToLocalChecked();
  Nan::Set(target, Nan::New("FunctionPassManager").ToLocalChecked(),
           constructorFunction);
}

NAN_METHOD(FunctionPassManagerWrapper::New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError(
        "Class Constructor FunctionPassManager cannot be invoked "
        "without new");
  }

  if (info.Length() != 1 || !ModuleWrapper::isInstance(info[0])) {
    return Nan::ThrowTypeError(
        "The FunctionPassManager functionTemplate needs to be called with: "
        "(module: Module)");
  }

  auto *module = ModuleWrapper::FromValue(info[0])->getModule();
  auto *wrapper = new FunctionPassManagerWrapper{module};
  wrapper->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate>
    &FunctionPassManagerWrapper::fpmTemplate() {
  static Nan::Persistent<v8::FunctionTemplate> tmpl{};

  if (tmpl.IsEmpty()) {
    v8::Local<v8::FunctionTemplate> localTemplate =
        Nan::New<v8::FunctionTemplate>(FunctionPassManagerWrapper::New);
    v8::Local<v8::FunctionTemplate> pmTemplate =
        Nan::New(PassManagerWrapper::pmTemplate());

    localTemplate->SetClassName(
        Nan::New("FunctionPassManager").ToLocalChecked());
    localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    localTemplate->Inherit(pmTemplate);

    Nan::SetPrototypeMethod(localTemplate, "add",
                            FunctionPassManagerWrapper::add);
    Nan::SetPrototypeMethod(localTemplate, "run",
                            FunctionPassManagerWrapper::run);
    Nan::SetPrototypeMethod(localTemplate, "doInitialization",
                            FunctionPassManagerWrapper::doInitialization);
    Nan::SetPrototypeMethod(localTemplate, "doFinalization",
                            FunctionPassManagerWrapper::doFinalization);
    tmpl.Reset(localTemplate);
  }

  return tmpl;
}

NAN_METHOD(FunctionPassManagerWrapper::add) {
  if (info.Length() != 1 || !FunctionPassManagerWrapper::isInstance(info[0])) {
    return Nan::ThrowTypeError("addk needs to be called with: p: Pass");
  }

  auto *wrapper = FunctionPassManagerWrapper::FromValue(info.Holder());
  auto *pass = PassWrapper::FromValue(info[0])->getPass();
  wrapper->fpm->add(pass);
}

NAN_METHOD(FunctionPassManagerWrapper::run) {
  if (info.Length() != 1 || !FunctionWrapper::isInstance(info[0])) {
    return Nan::ThrowTypeError("run needs to be called with: f: Function");
  }

  auto *wrapper = FunctionPassManagerWrapper::FromValue(info.Holder());
  auto *function = FunctionWrapper::FromValue(info[0])->getFunction();
  bool result = wrapper->fpm->run(*function);
  info.GetReturnValue().Set(Nan::New(result));
}

NAN_METHOD(FunctionPassManagerWrapper::doInitialization) {
  auto *wrapper = FunctionPassManagerWrapper::FromValue(info.Holder());
  bool result = wrapper->fpm->doInitialization();
  info.GetReturnValue().Set(Nan::New(result));
}

NAN_METHOD(FunctionPassManagerWrapper::doFinalization) {
  auto *wrapper = FunctionPassManagerWrapper::FromValue(info.Holder());
  bool result = wrapper->fpm->doFinalization();
  info.GetReturnValue().Set(Nan::New(result));
}

llvm::legacy::FunctionPassManager *
FunctionPassManagerWrapper::getFunctionPassManager() {
  return fpm;
}

bool FunctionPassManagerWrapper::isInstance(v8::Local<v8::Value> value) {
  return Nan::New(fpmTemplate())->HasInstance(value);
}

// Global pass creation functions.

NAN_METHOD(createInstructionCombiningPass) {
  auto *fnPass = llvm::createInstructionCombiningPass();
  info.GetReturnValue().Set(FunctionPassWrapper::of(fnPass));
}

NAN_METHOD(createReassociatePass) {
  auto *fnPass = llvm::createReassociatePass();
  info.GetReturnValue().Set(FunctionPassWrapper::of(fnPass));
}

NAN_METHOD(createGVNPass) {
  auto *fnPass = llvm::createGVNPass();
  info.GetReturnValue().Set(FunctionPassWrapper::of(fnPass));
}

NAN_METHOD(createCFGSimplificationPass) {
  auto *fnPass = llvm::createCFGSimplificationPass();
  info.GetReturnValue().Set(FunctionPassWrapper::of(fnPass));
}

NAN_MODULE_INIT(InitPasses) {
  Nan::Set(
      target,
      Nan::New<v8::String>("createInstructionCombiningPass").ToLocalChecked(),
      Nan::GetFunction(
          Nan::New<v8::FunctionTemplate>(createInstructionCombiningPass))
          .ToLocalChecked());

  Nan::Set(
      target, Nan::New<v8::String>("createReassociatePass").ToLocalChecked(),
      Nan::GetFunction(Nan::New<v8::FunctionTemplate>(createReassociatePass))
          .ToLocalChecked());

  Nan::Set(target, Nan::New<v8::String>("createGVNPass").ToLocalChecked(),
           Nan::GetFunction(Nan::New<v8::FunctionTemplate>(createGVNPass))
               .ToLocalChecked());

  Nan::Set(target,
           Nan::New<v8::String>("createCFGSimplificationPass").ToLocalChecked(),
           Nan::GetFunction(
               Nan::New<v8::FunctionTemplate>(createCFGSimplificationPass))
               .ToLocalChecked());
}