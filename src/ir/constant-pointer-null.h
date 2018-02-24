//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_CONSTANT_NULL_POINTER_H
#define LLVM_NODE_CONSTANT_NULL_POINTER_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Constants.h>
#include "constant.h"
#include "../util/from-value-mixin.h"

class ConstantPointerNullWrapper: public ConstantWrapper, public FromValueMixin<ConstantPointerNullWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::ConstantPointerNull* constantPointerNull);
    using FromValueMixin<ConstantPointerNullWrapper>::FromValue;
    llvm::ConstantPointerNull* getConstantPointerNull();

private:
    explicit ConstantPointerNullWrapper(llvm::ConstantPointerNull* constant)
            : ConstantWrapper { constant }
    {}

    static Napi::FunctionReference& constantPointerNullTemplate();

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value get(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_CONSTANT_NULL_POINTER_H
