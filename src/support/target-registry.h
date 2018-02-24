#ifndef TARGET_REGISTRY_H
#define TARGET_REGISTRY_H

#include <napi.h>
#include <uv.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include "../util/from-value-mixin.h"

class TargetRegistryWrapper : public Napi::ObjectWrap<TargetRegistryWrapper>, public FromValueMixin<TargetRegistryWrapper> {
    public:

    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    private:
        static Napi::Value lookupTarget(const Napi::CallbackInfo& info);

        TargetRegistryWrapper() = delete;
};

class TargetWrapper : public Napi::ObjectWrap<TargetWrapper>, public FromValueMixin<TargetWrapper> {
    public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(const llvm::Target *target);

    private:
    const llvm::Target* target {};

    explicit TargetWrapper(const llvm::Target* target): target { target } {
        assert(target && "No target pointer passed");
    }

    static Napi::Value createTargetMachine(const Napi::CallbackInfo& info);
    Napi::Value getName(const Napi::CallbackInfo& info);
    Napi::Value getShortDescription(const Napi::CallbackInfo& info);
    static Napi::Persistent<v8::ObjectTemplate> target_template;
};

#endif