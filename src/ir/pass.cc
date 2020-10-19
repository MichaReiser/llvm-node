//
// Created by lx.codev on 10.07.2020.
//

#include "pass.h"

#include <llvm/Support/raw_ostream.h>

#include "../util/string.h"
#include "llvm-context.h"
#include "function.h"
#include "module.h"
#include "pass-manager.h"

PassWrapper::PassWrapper(llvm::Pass *pass) : pass{pass} {}

NAN_MODULE_INIT(PassWrapper::Init) {
  auto constructorFunction =
      Nan::GetFunction(Nan::New(passTemplate())).ToLocalChecked();
  Nan::Set(target, Nan::New("Pass").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(PassWrapper::New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError(
        "Class Constructor Pass cannot be invoked without new");
  }

  if (info.Length() != 2 || !info[0]->IsExternal() || !info[1]->IsString()) {
    return Nan::ThrowTypeError(
        "The Pass functionTemplate needs to be called with: "
        "(k: PassKind, pid: string)");
  }

  auto *pass = static_cast<llvm::Pass *>(v8::External::Cast(*info[0])->Value());
  auto *wrapper = new PassWrapper{pass};
  wrapper->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &PassWrapper::passTemplate() {
  static Nan::Persistent<v8::FunctionTemplate> tmpl{};

  if (tmpl.IsEmpty()) {
    v8::Local<v8::FunctionTemplate> localTemplate =
        Nan::New<v8::FunctionTemplate>(PassWrapper::New);
    localTemplate->SetClassName(Nan::New("Pass").ToLocalChecked());
    localTemplate->InstanceTemplate()->SetInternalFieldCount(1);

    // Nan::SetMethod(localTemplate, "createPass", PassWrapper::createPass);
    Nan::SetAccessor(localTemplate->InstanceTemplate(),
                     Nan::New("passKind").ToLocalChecked(),
                     PassWrapper::getPassKind);
    Nan::SetAccessor(localTemplate->InstanceTemplate(),
                     Nan::New("passName").ToLocalChecked(),
                     PassWrapper::getPassName);
    Nan::SetAccessor(localTemplate->InstanceTemplate(),
                     Nan::New("passID").ToLocalChecked(),
                     PassWrapper::getPassID);
    Nan::SetPrototypeMethod(localTemplate, "doInitialization",
                            PassWrapper::doInitialization);
    Nan::SetPrototypeMethod(localTemplate, "doFinalization",
                            PassWrapper::doFinalization);
    Nan::SetPrototypeMethod(localTemplate, "print", PassWrapper::print);
    // TODO: add AnalysisResolverWrapper firstly.
    // Nan::SetAccessor(localTemplate->InstanceTemplate(),
    //                  Nan::New("resolver").ToLocalChecked(),
    //                  PassWrapper::getResolver,
    //                  PassWrapper::setResolver);
    Nan::SetAccessor(localTemplate->InstanceTemplate(),
                     Nan::New("potentialPassManagerType").ToLocalChecked(),
                     PassWrapper::getPotentialPassManagerType);
    // Nan::SetPrototypeMethod(localTemplate, "createPrinterPass",
    //                         PassWrapper::createPrinterPass);
    // Nan::SetPrototypeMethod(localTemplate, "assignPassManager",
    //                         PassWrapper::assignPassManager);
    Nan::SetPrototypeMethod(localTemplate, "verifyAnalysis",
                            PassWrapper::verifyAnalysis);

    tmpl.Reset(localTemplate);
  }

  return tmpl;
}

// NAN_METHOD(PassWrapper::createPass) {
//   if (info.Length() != 1 || !info[0]->IsUint32()) {
//     return Nan::ThrowTypeError(
//         "Create needs to be called with: id: AnalysisId");
//   }

//   auto *analysisId = Nan::To<uint32_t>(info[1]).FromJust();
//   auto *pass = llvm::Pass::create(analysisId);
//   auto wrapper = of(pass);
//   info.GetReturnValue().Set(wrapper);
// }

NAN_GETTER(PassWrapper::getPassKind) {
  auto *wrapper = PassWrapper::FromValue(info.Holder());
  auto passKind = wrapper->getPass()->getPassKind();
  info.GetReturnValue().Set(Nan::New(passKind));
}

NAN_GETTER(PassWrapper::getPassName) {
  auto *wrapper = PassWrapper::FromValue(info.Holder());
  auto passName = Nan::New(wrapper->getPass()->getPassName().str()).ToLocalChecked();
  info.GetReturnValue().Set(passName);
}

NAN_GETTER(PassWrapper::getPassID) {
  auto *wrapper = PassWrapper::FromValue(info.Holder());
  auto id = wrapper->getPass()->getPassID();
  info.GetReturnValue().Set(Nan::New(id));
}

NAN_METHOD(PassWrapper::doInitialization) {
  if (info.Length() != 1 || !ModuleWrapper::isInstance(info[0])) {
    return Nan::ThrowTypeError(
        "doInitialization needs to be called with: m: Module");
  }

  auto *wrapper = PassWrapper::FromValue(info.Holder());
  auto *module = ModuleWrapper::FromValue(info[0])->getModule();
  bool result = wrapper->getPass()->doInitialization(*module);
  info.GetReturnValue().Set(Nan::New(result));
}

NAN_METHOD(PassWrapper::doFinalization) {
  if (info.Length() != 1 || !ModuleWrapper::isInstance(info[0])) {
    return Nan::ThrowTypeError(
        "doFinalization needs to be called with: m: Module");
  }

  auto *wrapper = PassWrapper::FromValue(info.Holder());
  auto *module = ModuleWrapper::FromValue(info[0])->getModule();
  bool result = wrapper->getPass()->doFinalization(*module);
  info.GetReturnValue().Set(Nan::New(result));
}

NAN_METHOD(PassWrapper::print) {
  if (info.Length() != 1 || !ModuleWrapper::isInstance(info[0])) {
    return Nan::ThrowTypeError("run needs to be called with: m: Module");
  }

  auto *wrapper = PassWrapper::FromValue(info.Holder());
  auto *module = ModuleWrapper::FromValue(info[0])->getModule();

  std::string outs;
  llvm::raw_string_ostream os(outs);
  wrapper->getPass()->print(os, module);
  os.flush();
  auto result = Nan::New<v8::String>(outs).ToLocalChecked();
  info.GetReturnValue().Set(result);
}

NAN_GETTER(PassWrapper::getPotentialPassManagerType) {
  auto *wrapper = PassWrapper::FromValue(info.Holder());
  bool type = wrapper->pass->getPotentialPassManagerType();
  info.GetReturnValue().Set(type);
}

NAN_METHOD(PassWrapper::verifyAnalysis) {
  auto *wrapper = PassWrapper::FromValue(info.Holder());
  wrapper->getPass()->verifyAnalysis();
}

llvm::Pass *PassWrapper::getPass() { return pass; }

bool PassWrapper::isInstance(v8::Local<v8::Value> value) {
  return Nan::New(passTemplate())->HasInstance(value);
}

// ModulePassWrapper

ModulePassWrapper::ModulePassWrapper(llvm::ModulePass *mp) : PassWrapper{mp} {}

NAN_MODULE_INIT(ModulePassWrapper::Init) {
  auto constructorFunction =
      Nan::GetFunction(Nan::New(mpTemplate())).ToLocalChecked();
  Nan::Set(target, Nan::New("ModulePass").ToLocalChecked(),
           constructorFunction);
}

NAN_METHOD(ModulePassWrapper::New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError(
        "Class Constructor ModulePass cannot be invoked without new");
  }

  if (info.Length() != 1 || !info[0]->IsExternal()) {
    return Nan::ThrowTypeError("External ModulePass Pointer required");
  }

  auto *mp =
      static_cast<llvm::ModulePass *>(v8::External::Cast(*info[0])->Value());
  auto *wrapper = new ModulePassWrapper{mp};
  wrapper->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &ModulePassWrapper::mpTemplate() {
  static Nan::Persistent<v8::FunctionTemplate> tmpl{};

  if (tmpl.IsEmpty()) {
    v8::Local<v8::FunctionTemplate> localTemplate =
        Nan::New<v8::FunctionTemplate>(ModulePassWrapper::New);
    v8::Local<v8::FunctionTemplate> passTemplate =
        Nan::New(PassWrapper::passTemplate());

    localTemplate->SetClassName(Nan::New("ModulePass").ToLocalChecked());
    localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    localTemplate->Inherit(passTemplate);

    Nan::SetPrototypeMethod(localTemplate, "runOnModule",
                            ModulePassWrapper::runOnModule);

    tmpl.Reset(localTemplate);
  }

  return tmpl;
}

NAN_METHOD(ModulePassWrapper::runOnModule) {
  if (info.Length() != 1 || !ModuleWrapper::isInstance(info[0])) {
    return Nan::ThrowTypeError(
        "runOnModule needs to be called with: m: Module");
  }

  auto *wrapper = ModulePassWrapper::FromValue(info.Holder());
  auto *module = ModuleWrapper::FromValue(info[0])->getModule();
  bool result = wrapper->getModulePass()->runOnModule(*module);
  info.GetReturnValue().Set(Nan::New(result));
}

llvm::ModulePass *ModulePassWrapper::getModulePass() {
  return static_cast<llvm::ModulePass *>(getPass());
}

bool ModulePassWrapper::isInstance(v8::Local<v8::Value> value) {
  return Nan::New(mpTemplate())->HasInstance(value);
}

// FunctionPassWrapper

FunctionPassWrapper::FunctionPassWrapper(llvm::FunctionPass *fp)
    : PassWrapper{fp} {}

NAN_MODULE_INIT(FunctionPassWrapper::Init) {
  auto constructorFunction =
      Nan::GetFunction(Nan::New(fpTemplate())).ToLocalChecked();
  Nan::Set(target, Nan::New("FunctionPass").ToLocalChecked(),
           constructorFunction);
}

NAN_METHOD(FunctionPassWrapper::New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError(
        "Class Constructor FunctionPass cannot be invoked without new");
  }

  if (info.Length() != 1 || !info[0]->IsExternal()) {
    return Nan::ThrowTypeError("External FunctionPass Pointer required");
  }

  auto *fp =
      static_cast<llvm::FunctionPass *>(v8::External::Cast(*info[0])->Value());
  auto *wrapper = new FunctionPassWrapper{fp};
  wrapper->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &FunctionPassWrapper::fpTemplate() {
  static Nan::Persistent<v8::FunctionTemplate> tmpl{};

  if (tmpl.IsEmpty()) {
    v8::Local<v8::FunctionTemplate> localTemplate =
        Nan::New<v8::FunctionTemplate>(FunctionPassWrapper::New);
    v8::Local<v8::FunctionTemplate> passTemplate =
        Nan::New(PassWrapper::passTemplate());

    localTemplate->SetClassName(Nan::New("FunctionPass").ToLocalChecked());
    localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    localTemplate->Inherit(passTemplate);

    Nan::SetPrototypeMethod(localTemplate, "runOnFunction",
                            FunctionPassWrapper::runOnFunction);

    tmpl.Reset(localTemplate);
  }

  return tmpl;
}

NAN_METHOD(FunctionPassWrapper::runOnFunction) {
  if (info.Length() != 1 || !FunctionWrapper::isInstance(info[0])) {
    return Nan::ThrowTypeError(
        "runOnFunction needs to be called with: f: Function");
  }

  auto *wrapper = FunctionPassWrapper::FromValue(info.Holder());
  auto *function = FunctionWrapper::FromValue(info[0])->getFunction();
  bool result = wrapper->getFunctionPass()->runOnFunction(*function);
  info.GetReturnValue().Set(Nan::New(result));
}

llvm::FunctionPass *FunctionPassWrapper::getFunctionPass() {
  return static_cast<llvm::FunctionPass *>(getPass());
}

bool FunctionPassWrapper::isInstance(v8::Local<v8::Value> value) {
  return Nan::New(fpTemplate())->HasInstance(value);
}
