#ifndef LLVM_NODE_DI_SCOPE_H
#define LLVM_NODE_DI_SCOPE_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <nan_callbacks_12_inl.h>
#include "../util/from-value-mixin.h"

class DIScopeWrapper: public Nan::ObjectWrap, public FromValueMixin<DIScopeWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::DIScope *scope);

    static bool isInstance(v8::Local<v8::Value> value);

    llvm::DIScope *getDIScope();

protected:
    explicit DIScopeWrapper(llvm::DIScope *scope) : scope { scope } {
    }

    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;

private:
    llvm::DIScope *scope;

    static NAN_METHOD(New);

    // instance
    static NAN_GETTER(getFilename);
    static NAN_GETTER(getDirectory);
    static NAN_GETTER(getName);
};

#endif //LLVM_NODE_DI_SCOPE_H
