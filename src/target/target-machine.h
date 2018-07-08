//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_TARGET_MACHINE_H
#define LLVM_NODE_TARGET_MACHINE_H

#include <napi.h>
#include <llvm/Target/TargetMachine.h>

class TargetMachineWrapper: public Napi::ObjectWrap<TargetMachineWrapper> {
public:
    static void Init(Napi::Env env, Napi::Object& exports);
    static Napi::Object of(Napi::Env env, llvm::TargetMachine* machine);
    static bool isInstanceOfType(const Napi::Value& value);
    explicit TargetMachineWrapper(const Napi::CallbackInfo& info);

private:
    static Napi::FunctionReference constructor;
    const llvm::TargetMachine* targetMachine;


//    Napi::Value createDataLayout(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_TARGET_MACHINE_H
