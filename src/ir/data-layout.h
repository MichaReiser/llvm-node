//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_DATA_LAYOUT_H
#define LLVM_NODE_DATA_LAYOUT_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/DataLayout.h>
#include "../util/from-value-mixin.h"

class DataLayoutWrapper : public Napi::ObjectWrap<DataLayoutWrapper>, public FromValueMixin<DataLayoutWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::DataLayout layout);
    static bool isInstance(Napi::Value value);
    llvm::DataLayout getDataLayout();

private:
    static Napi::FunctionReference functionTemplate;
    llvm::DataLayout layout;

    explicit DataLayoutWrapper(llvm::DataLayout layout): layout { layout } {
    }

    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value getStringRepresentation(const Napi::CallbackInfo& info);
    static Napi::Value getPointerSize(const Napi::CallbackInfo& info);
    static Napi::Value getPrefTypeAlignment(const Napi::CallbackInfo& info);
    static Napi::Value getTypeStoreSize(const Napi::CallbackInfo& info);
    static Napi::Value getIntPtrType(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_DATA_LAYOUT_H
