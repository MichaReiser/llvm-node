#ifndef LLVM_NODE_DI_FILE_H
#define LLVM_NODE_DI_FILE_H

#include <nan.h>
#include <llvm/IR/DebugInfoMetadata.h>
#include <nan_callbacks_12_inl.h>
#include "../util/from-value-mixin.h"

class DIFileWrapper: public Nan::ObjectWrap, public FromValueMixin<DIFileWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::DIFile *diFile);

    static bool isInstance(v8::Local<v8::Value> value);

    llvm::DIFile *getDIFile();

private:
    llvm::DIFile *diFile;

    explicit DIFileWrapper(llvm::DIFile *diFile) : diFile { diFile } {
    }

    static NAN_METHOD(New);

    // instance
    static NAN_GETTER(getFilename);
    static NAN_GETTER(getDirectory);

    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
};

#endif //LLVM_NODE_DI_FILE_H
