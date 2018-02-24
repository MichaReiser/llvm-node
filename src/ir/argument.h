//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_ARGUMENT_H
#define LLVM_NODE_ARGUMENT_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Argument.h>
#include "value.h"
#include "../util/from-value-mixin.h"

class ArgumentWrapper: public ValueWrapper, public FromValueMixin<ArgumentWrapper> {
public:
    llvm::Argument* getArgument();

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::Argument* argument);
    using FromValueMixin<ArgumentWrapper>::FromValue;

private:
    explicit ArgumentWrapper(llvm::Argument *argument)
        : ValueWrapper { argument }
    {}

    static Napi::Value New(const Napi::CallbackInfo& info);
    Napi::Value getArgNo(const Napi::CallbackInfo& info);
    Napi::Value getParent(const Napi::CallbackInfo& info);
    static Napi::Value addAttr(const Napi::CallbackInfo& info);
    static Napi::Value hasAttribute(const Napi::CallbackInfo& info);
    static Napi::Value addDereferenceableAttr(const Napi::CallbackInfo& info);

    static inline Napi::FunctionReference& argumentTemplate() {
        static Napi::FunctionReference functionTemplate {};
        return functionTemplate;
    }
};


#endif //LLVM_NODE_ARGUMENT_H
