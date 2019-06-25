#ifndef LLVM_NODE_DI_BUILDER_H
#define LLVM_NODE_DI_BUILDER_H

#include <nan.h>
#include <llvm/IR/DIBuilder.h>
#include <nan_callbacks_12_inl.h>
#include "../util/from-value-mixin.h"
#include "../util/string.h"

class DIBuilderWrapper: public Nan::ObjectWrap, public FromValueMixin<DIBuilderWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    llvm::DIBuilder *getDIBuilder();

private:
    llvm::DIBuilder *diBuilder;

    explicit DIBuilderWrapper(llvm::DIBuilder *diBuilder) : diBuilder { diBuilder } {
    }

    // static
    static NAN_METHOD(New);

    // instance
    static NAN_METHOD(CreateFile);
    static NAN_METHOD(CreateCompileUnit);
    static NAN_METHOD(CreateBasicType);
    static NAN_METHOD(CreateSubroutineType);
    static NAN_METHOD(Finalize);

    static inline Nan::Persistent<v8::Function>& diBuilderConstructor() {
        static Nan::Persistent<v8::Function> constructor {};
        return constructor;
    }
};

#endif //LLVM_NODE_DI_BUILDER_H
