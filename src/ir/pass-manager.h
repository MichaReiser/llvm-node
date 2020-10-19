//
// Created by lx.codev on 10.07.2020.
//

#ifndef LLVM_NODE_PASS_MANAGER_H
#define LLVM_NODE_PASS_MANAGER_H

#include <llvm/IR/LegacyPassManager.h>
#include <nan.h>

#include "../util/from-value-mixin.h"

class PassManagerWrapper : public Nan::ObjectWrap,
                           public FromValueMixin<PassManagerWrapper> {
 public:
  static NAN_MODULE_INIT(Init);

  llvm::legacy::PassManager *getPassManager();

  static bool isInstance(v8::Local<v8::Value> value);

 protected:
  static Nan::Persistent<v8::FunctionTemplate> &pmTemplate();

  explicit PassManagerWrapper() : pm{new llvm::legacy::PassManager{}} {}

 private:
  llvm::legacy::PassManager *pm;

  // static
  static NAN_METHOD(New);

  // instance
  static NAN_METHOD(add);
  static NAN_METHOD(run);
};

class FunctionPassManagerWrapper
    : public PassManagerWrapper,
      public FromValueMixin<FunctionPassManagerWrapper> {
  using FromValueMixin<FunctionPassManagerWrapper>::FromValue;

 public:
  static NAN_MODULE_INIT(Init);

  llvm::legacy::FunctionPassManager *getFunctionPassManager();

  static bool isInstance(v8::Local<v8::Value> value);

 private:
  llvm::legacy::FunctionPassManager *fpm;

  FunctionPassManagerWrapper(llvm::Module *module)
      : fpm{new llvm::legacy::FunctionPassManager{module}} {}

  static Nan::Persistent<v8::FunctionTemplate> &fpmTemplate();

  // static
  static NAN_METHOD(New);

  // instance
  static NAN_METHOD(add);
  static NAN_METHOD(run);
  static NAN_METHOD(doInitialization);
  static NAN_METHOD(doFinalization);
};

NAN_METHOD(createInstructionCombiningPass);
NAN_METHOD(createReassociatePass);
NAN_METHOD(createGVNPass);
NAN_METHOD(createCFGSimplificationPass);

NAN_MODULE_INIT(InitPasses);

#endif  // LLVM_NODE_PASS_MANAGER_H
