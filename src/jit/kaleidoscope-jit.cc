//
// Created by lx.codev on 10.07.2020.
//

#include "kaleidoscope-jit.h"

#include "../ir/module.h"
#include "../target/target-machine.h"
#include "../util/string.h"
#include "jit-symbol.h"

NAN_MODULE_INIT(KaleidoscopeJitWrapper::Init) {
  v8::Local<v8::FunctionTemplate> functionTemplate =
      Nan::New<v8::FunctionTemplate>(New);
  functionTemplate->SetClassName(Nan::New("KaleidoscopeJit").ToLocalChecked());
  functionTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetAccessor(functionTemplate->InstanceTemplate(),
                   Nan::New("targetMachine").ToLocalChecked(),
                   KaleidoscopeJitWrapper::targetMachine);
  Nan::SetPrototypeMethod(functionTemplate, "addModule",
                          KaleidoscopeJitWrapper::addModule);
  Nan::SetPrototypeMethod(functionTemplate, "removeModule",
                          KaleidoscopeJitWrapper::removeModule);
  Nan::SetPrototypeMethod(functionTemplate, "findSymbol",
                          KaleidoscopeJitWrapper::findSymbol);
  jitTemplate().Reset(functionTemplate);

  auto constructorFunction =
      Nan::GetFunction(functionTemplate).ToLocalChecked();
  Nan::Set(target, Nan::New("KaleidoscopeJit").ToLocalChecked(),
           constructorFunction);
}

NAN_METHOD(KaleidoscopeJitWrapper::New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError(
        "The KaleidoscopeJit functionTemplate needs to be called with new");
  }

  auto* wrapper = new KaleidoscopeJitWrapper{};
  wrapper->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

NAN_GETTER(KaleidoscopeJitWrapper::targetMachine) {
  auto* tm = &KaleidoscopeJitWrapper::FromValue(info.Holder())
                  ->jit->getTargetMachine();
  info.GetReturnValue().Set(TargetMachineWrapper::of(tm));
}

NAN_METHOD(KaleidoscopeJitWrapper::addModule) {
  if (info.Length() != 1 || !ModuleWrapper::isInstance(info[0])) {
    return Nan::ThrowTypeError("addModule needs to be called with: m: Module");
  }

  auto* wrapper = KaleidoscopeJitWrapper::FromValue(info.Holder());
  auto* module = ModuleWrapper::FromValue(info[0])->getModule();
  auto key = wrapper->jit->addModule(std::unique_ptr<llvm::Module>(module));
  info.GetReturnValue().Set(Nan::New(static_cast<uint32_t>(key)));
}

NAN_METHOD(KaleidoscopeJitWrapper::removeModule) {
  if (info.Length() != 1 || !info[0]->IsUint32()) {
    return Nan::ThrowTypeError(
        "removeModule needs to be called with: key: VModuleKey");
  }

  auto* wrapper = KaleidoscopeJitWrapper::FromValue(info.Holder());
  auto key =
      Nan::To<uint32_t>(info[0]).FromJust();  // v8 does not support uint64_t :(
  wrapper->jit->removeModule(key);
}

NAN_METHOD(KaleidoscopeJitWrapper::findSymbol) {
  if (info.Length() < 1 || !info[0]->IsString()) {
    return Nan::ThrowTypeError(
        "findSymbol needs to be called with: name: string");
  }

  auto* wrapper = KaleidoscopeJitWrapper::FromValue(info.Holder());
  const std::string name = ToString(info[0]);
  auto symbol = wrapper->jit->findSymbol(name);

  info.GetReturnValue().Set(JitSymbolWrapper::of(&symbol));
}

llvm::orc::KaleidoscopeJIT* KaleidoscopeJitWrapper::getKaleidoscopeJit() {
  return jit;
}

bool KaleidoscopeJitWrapper::isInstance(v8::Local<v8::Value> value) {
  return Nan::New(jitTemplate())->HasInstance(value);
}
