//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_TARGET_MACHINE_H
#define LLVM_NODE_TARGET_MACHINE_H

#include <napi.h>
#include <uv.h>
#include <llvm/Target/TargetMachine.h>
#include "../util/from-value-mixin.h"

class TargetMachineWrapper : public Napi::ObjectWrap<TargetMachineWrapper>, public FromValueMixin<TargetMachineWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(const llvm::TargetMachine *ptr);

private:
    const llvm::TargetMachine* targetMachine;
    static Napi::Persistent<v8::ObjectTemplate> targetMachineTemplate;

    explicit TargetMachineWrapper(const llvm::TargetMachine* targetMachine): targetMachine { targetMachine } {
        assert(targetMachine && "No target machine passed");
    }

    static Napi::Value createDataLayout(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_TARGET_MACHINE_H
