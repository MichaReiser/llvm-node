//
// Created by lx.codev on 10.07.2020.
//

#include "jit-symbol.h"

#include "../util/string.h"

NAN_MODULE_INIT(JitSymbolWrapper::Init) {
  auto constructorFunction =
      Nan::GetFunction(Nan::New(symbolTemplate())).ToLocalChecked();
  Nan::Set(target, Nan::New("JitSymbol").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(JitSymbolWrapper::New) {
  if (!info.IsConstructCall()) {
    return Nan::ThrowTypeError(
        "Class Constructor JitSymbol cannot be invoked without new");
  }

  if (info.Length() != 1 || !info[0]->IsExternal()) {
    return Nan::ThrowTypeError("External JitSymbol Pointer required");
  }

  auto *symbol =
      static_cast<llvm::JITSymbol *>(v8::External::Cast(*info[0])->Value());
  auto *wrapper = new JitSymbolWrapper{symbol};
  wrapper->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &JitSymbolWrapper::symbolTemplate() {
  static Nan::Persistent<v8::FunctionTemplate> tmpl{};

  if (tmpl.IsEmpty()) {
    v8::Local<v8::FunctionTemplate> localTemplate =
        Nan::New<v8::FunctionTemplate>(JitSymbolWrapper::New);
    localTemplate->SetClassName(Nan::New("JitSymbol").ToLocalChecked());
    localTemplate->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetAccessor(localTemplate->InstanceTemplate(),
                     Nan::New("address").ToLocalChecked(), address);
    Nan::SetPrototypeMethod(localTemplate, "call", JitSymbolWrapper::call);

    tmpl.Reset(localTemplate);
  }

  return tmpl;
}

NAN_GETTER(JitSymbolWrapper::address) {
  auto *symbol = JitSymbolWrapper::FromValue(info.Holder())->symbol;
  auto address = (double (*)())(intptr_t)cantFail(symbol->getAddress());
  info.GetReturnValue().Set(Nan::New(address));
}

NAN_METHOD(JitSymbolWrapper::call) {
  auto *symbol = JitSymbolWrapper::FromValue(info.Holder())->symbol;
  auto fp = (double (*)())(intptr_t)cantFail(symbol->getAddress());
  auto result = fp();
  info.GetReturnValue().Set(Nan::New(result));
}

llvm::JITSymbol *JitSymbolWrapper::getJitSymbol() { return symbol; }

bool JitSymbolWrapper::isInstance(v8::Local<v8::Value> value) {
  return Nan::New(symbolTemplate())->HasInstance(value);
}
