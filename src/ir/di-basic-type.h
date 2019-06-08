#ifndef LLVM_NODE_DI_BASIC_TYPE_H
#define LLVM_NODE_DI_BASIC_TYPE_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <nan_callbacks_12_inl.h>
#include "../util/from-value-mixin.h"
#include "di-type.h"

class DIBasicTypeWrapper: public DITypeWrapper, public FromValueMixin<DIBasicTypeWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::DIBasicType *type);

    using FromValueMixin<DIBasicTypeWrapper>::FromValue;

    static bool isInstance(v8::Local<v8::Value> value);

    llvm::DIBasicType *getDIBasicType();

private:
    explicit DIBasicTypeWrapper(llvm::DIBasicType *type) : DITypeWrapper { type } {
    }

    static NAN_METHOD(New);

    // instance
    static NAN_GETTER(getSignedness);

    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
};

#endif //LLVM_NODE_DI_BASIC_TYPE_H
