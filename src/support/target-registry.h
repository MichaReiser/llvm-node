#ifndef TARGET_REGISTRY_H
#define TARGET_REGISTRY_H

#include <napi.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>

void InitTargetRegistry(Napi::Env env, Napi::Object& exports);

class TargetWrapper: public Napi::ObjectWrap<TargetWrapper> {
    public:
    static void Init(Napi::Env env, Napi::Object& exports);
    explicit TargetWrapper(const Napi::CallbackInfo& info);

    static Napi::Object of(Napi::Env env, llvm::Target *target);

    private:
    static Napi::FunctionReference constructor;

    const llvm::Target* target {};

    Napi::Value createTargetMachine(const Napi::CallbackInfo& info);
    Napi::Value getName(const Napi::CallbackInfo& info);
    Napi::Value getShortDescription(const Napi::CallbackInfo& info);
};

#endif