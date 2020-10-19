//
// Created by lx.codev on 10.07.2020.
//

#ifndef LLVM_NODE_KALEIDOSCOPEJIT_H
#define LLVM_NODE_KALEIDOSCOPEJIT_H

#include <nan.h>

#include "../util/from-value-mixin.h"
#include "KaleidoscopeJIT.h"

class KaleidoscopeJitWrapper : public Nan::ObjectWrap,
                               public FromValueMixin<KaleidoscopeJitWrapper> {
 public:
  static NAN_MODULE_INIT(Init);

  llvm::orc::KaleidoscopeJIT *getKaleidoscopeJit();

  static bool isInstance(v8::Local<v8::Value> value);

 private:
  llvm::orc::KaleidoscopeJIT *jit;

  KaleidoscopeJitWrapper() : jit{new llvm::orc::KaleidoscopeJIT{}} {}

  static Nan::Persistent<v8::FunctionTemplate> &jitTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate{};
    return functionTemplate;
  }

  // static
  static NAN_METHOD(New);

  // instance
  static NAN_GETTER(targetMachine);
  static NAN_METHOD(addModule);
  static NAN_METHOD(removeModule);
  static NAN_METHOD(findSymbol);
};

#endif  // LLVM_NODE_KALEIDOSCOPEJIT_H
