//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_TARGET_MACHINE_H
#define LLVM_NODE_TARGET_MACHINE_H

#include <nan.h>
#include <llvm/Target/TargetMachine.h>
#include "../util/from-value-mixin.h"

class TargetMachineWrapper: public Nan::ObjectWrap, public FromValueMixin<TargetMachineWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> Create(const llvm::TargetMachine *ptr);

private:
    const llvm::TargetMachine* targetMachine;
    static Nan::Persistent<v8::ObjectTemplate> targetMachineTemplate;

    TargetMachineWrapper(const llvm::TargetMachine* targetMachine): targetMachine { targetMachine }
    {}

    static NAN_METHOD(createDataLayout);
};

#endif //LLVM_NODE_TARGET_MACHINE_H
