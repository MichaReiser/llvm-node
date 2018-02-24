//
// Created by Micha Reiser on 04.04.17.
//

#ifndef LLVM_NODE_CONSTANT_ARRAY_H
#define LLVM_NODE_CONSTANT_ARRAY_H

#include <napi.h>
#include <uv.h>
#include "../util/from-value-mixin.h"
#include "constant.h"

class ConstantArrayWrapper: public ConstantWrapper, public FromValueMixin<ConstantArrayWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::ConstantArray* constantArray);
    using FromValueMixin<ConstantArrayWrapper>::FromValue;

private:
    explicit ConstantArrayWrapper(llvm::ConstantArray* array)
            : ConstantWrapper { array }
    {}

    static Napi::FunctionReference& constantArrayTemplate();

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value get(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_CONSTANT_ARRAY_H
