//
// Created by lx.codev on 10.07.2020.
//

#ifndef LLVM_NODE_JIT_SYMBOL_H
#define LLVM_NODE_JIT_SYMBOL_H

#include <llvm/ExecutionEngine/JITSymbol.h>
#include <nan.h>

#include "../util/from-value-mixin.h"

class JitSymbolWrapper : public Nan::ObjectWrap,
                         public FromValueMixin<JitSymbolWrapper> {
 public:
  static NAN_MODULE_INIT(Init);
  static v8::Local<v8::Object> of(llvm::JITSymbol *symbol);

  llvm::JITSymbol *getJitSymbol();

  static bool isInstance(v8::Local<v8::Value> value);
  using FromValueMixin<JitSymbolWrapper>::FromValue;

 private:
  llvm::JITSymbol *symbol;

  JitSymbolWrapper(llvm::JITSymbol *symbol) : symbol{symbol} {}

  static Nan::Persistent<v8::FunctionTemplate> &symbolTemplate();

  // static
  static NAN_METHOD(New);

  // instance
  static NAN_GETTER(address);
  static NAN_METHOD(call);
};

#endif  // LLVM_NODE_JIT_SYMBOL_H
