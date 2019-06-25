#ifndef LLVM_NODE_DI_SUBROUTINE_TYPE_H
#define LLVM_NODE_DI_SUBROUTINE_TYPE_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <nan_callbacks_12_inl.h>
#include "../util/from-value-mixin.h"
#include "di-type.h"

class DISubroutineTypeWrapper: public DITypeWrapper, public FromValueMixin<DISubroutineTypeWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::DISubroutineType *type);

    using FromValueMixin<DISubroutineTypeWrapper>::FromValue;

    static bool isInstance(v8::Local<v8::Value> value);

    llvm::DISubroutineType *getDISubroutineType();

private:
    explicit DISubroutineTypeWrapper(llvm::DISubroutineType *type) : DITypeWrapper { type } {
    }

    static NAN_METHOD(New);

    // instance
    static NAN_GETTER(getParameters);

    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
};

#endif //LLVM_NODE_DI_SUBROUTINE_TYPE_H
