//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_DATA_LAYOUT_H
#define LLVM_NODE_DATA_LAYOUT_H

#include <nan.h>
#include <llvm/IR/DataLayout.h>
#include "../util/from-value-mixin.h"

class DataLayoutWrapper: public Nan::ObjectWrap, public FromValueMixin<DataLayoutWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::DataLayout layout);
    static bool isInstance(v8::Local<v8::Value> value);
    llvm::DataLayout getDataLayout();

private:
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate;
    llvm::DataLayout layout;

    explicit DataLayoutWrapper(llvm::DataLayout layout): layout { layout } {
    }

    static NAN_METHOD(New);
    static NAN_METHOD(getStringRepresentation);
    static NAN_METHOD(getPointerSize);
    static NAN_METHOD(getPrefTypeAlignment);
    static NAN_METHOD(getTypeStoreSize);
    static NAN_METHOD(getIntPtrType);
};

#endif //LLVM_NODE_DATA_LAYOUT_H
