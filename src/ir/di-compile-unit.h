#ifndef LLVM_NODE_DI_COMPILE_UNIT_H
#define LLVM_NODE_DI_COMPILE_UNIT_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <nan_callbacks_12_inl.h>
#include "../util/from-value-mixin.h"

class DICompileUnitWrapper: public Nan::ObjectWrap, public FromValueMixin<DICompileUnitWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::DICompileUnit *compileUnit);

    static bool isInstance(v8::Local<v8::Value> value);

    llvm::DICompileUnit *getDICompileUnit();

private:
    llvm::DICompileUnit *compileUnit;

    explicit DICompileUnitWrapper(llvm::DICompileUnit *compileUnit) : compileUnit { compileUnit } {
    }

    static NAN_METHOD(New);

    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
};

#endif //LLVM_NODE_DI_COMPILE_UNIT_H
