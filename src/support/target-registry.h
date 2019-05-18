#ifndef TARGET_REGISTRY_H
#define TARGET_REGISTRY_H

#include <nan.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include "../util/from-value-mixin.h"

class TargetRegistryWrapper: public Nan::ObjectWrap, public FromValueMixin<TargetRegistryWrapper> {
    public:
    static NAN_MODULE_INIT(Init);

    private:
    static NAN_METHOD(lookupTarget);

    TargetRegistryWrapper() = delete;
};

class TargetWrapper: public Nan::ObjectWrap, public FromValueMixin<TargetWrapper> {
    public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(const llvm::Target *target);

    private:
    const llvm::Target* target {};

    explicit TargetWrapper(const llvm::Target* target): target { target } {
        assert(target && "No target pointer passed");
    }

    static NAN_METHOD(createTargetMachine);
    static NAN_GETTER(getName);
    static NAN_GETTER(getShortDescription);
    static Nan::Persistent<v8::ObjectTemplate> target_template;
};

#endif
