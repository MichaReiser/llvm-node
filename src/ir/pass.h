//
// Created by lx.codev on 10.07.2020.
//

#ifndef LLVM_NODE_PASS_H
#define LLVM_NODE_PASS_H

#include <llvm/Pass.h>
#include <nan.h>

#include "../util/from-value-mixin.h"

class PassWrapper : public Nan::ObjectWrap, public FromValueMixin<PassWrapper> {
 public:
  static NAN_MODULE_INIT(Init);
  static v8::Local<v8::Object> of(llvm::Pass *pass);

  llvm::Pass *getPass();

  static bool isInstance(v8::Local<v8::Value> value);
  using FromValueMixin<PassWrapper>::FromValue;

 protected:
  static Nan::Persistent<v8::FunctionTemplate> &passTemplate();

  explicit PassWrapper(llvm::Pass *pass);

 private:
  llvm::Pass *pass;

  // static
  static NAN_METHOD(New);
  static NAN_METHOD(createPass);

  // instance
  static NAN_GETTER(getPassKind);
  static NAN_GETTER(getPassName);
  static NAN_GETTER(getPassID);
  static NAN_METHOD(doInitialization);
  static NAN_METHOD(doFinalization);
  static NAN_METHOD(print);
  // static NAN_METHOD(createPrinterPass);
  // static NAN_METHOD(assignPassManager);
  // static NAN_SETTER(setResolver);
  // static NAN_GETTER(getResolver);
  static NAN_GETTER(getPotentialPassManagerType);
  static NAN_METHOD(verifyAnalysis);
};

class ModulePassWrapper : public PassWrapper,
                          public FromValueMixin<ModulePassWrapper> {
 public:
  static NAN_MODULE_INIT(Init);
  static v8::Local<v8::Object> of(llvm::ModulePass *pass);

  llvm::ModulePass *getModulePass();

  static bool isInstance(v8::Local<v8::Value> value);
  using FromValueMixin<ModulePassWrapper>::FromValue;

 protected:
  static Nan::Persistent<v8::FunctionTemplate> &mpTemplate();

  explicit ModulePassWrapper(llvm::ModulePass *mp);

 private:
  // static
  static NAN_METHOD(New);
  static NAN_METHOD(createPass);

  // instance
  // static NAN_METHOD(createPrinterPass);
  static NAN_METHOD(runOnModule);
  // static NAN_METHOD(assignPassManager);
};

class FunctionPassWrapper : public PassWrapper,
                            public FromValueMixin<FunctionPassWrapper> {
 public:
  static NAN_MODULE_INIT(Init);
  static v8::Local<v8::Object> of(llvm::FunctionPass *pass);

  llvm::FunctionPass *getFunctionPass();

  static bool isInstance(v8::Local<v8::Value> value);
  using FromValueMixin<FunctionPassWrapper>::FromValue;

 protected:
  static Nan::Persistent<v8::FunctionTemplate> &fpTemplate();

  explicit FunctionPassWrapper(llvm::FunctionPass *fp);

 private:
  // static
  static NAN_METHOD(New);
  static NAN_METHOD(createPass);

  // instance
  // static NAN_METHOD(createPrinterPass);
  static NAN_METHOD(runOnFunction);
  // static NAN_METHOD(assignPassManager);
};

#endif  // LLVM_NODE_PASS_H
