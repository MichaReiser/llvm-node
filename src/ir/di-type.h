#ifndef LLVM_NODE_DI_TYPE_H
#define LLVM_NODE_DI_TYPE_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <nan_callbacks_12_inl.h>
#include "../util/from-value-mixin.h"
#include "di-scope.h"

class DITypeWrapper: public DIScopeWrapper, public FromValueMixin<DITypeWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::DIType *type);

    using FromValueMixin<DITypeWrapper>::FromValue;

    static bool isInstance(v8::Local<v8::Value> value);

    llvm::DIType *getDIType();

protected:
    explicit DITypeWrapper(llvm::DIType *type) : DIScopeWrapper { type } {
    }

    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;

private:
    static NAN_METHOD(New);

    // instance
    static NAN_GETTER(getLine);
};

#endif //LLVM_NODE_DI_TYPE_H
