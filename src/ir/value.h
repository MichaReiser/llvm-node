//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_VALUE_H
#define LLVM_NODE_VALUE_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Value.h>
#include "../util/from-value-mixin.h"

class ValueWrapper : public Napi::ObjectWrap<ValueWrapper>, public FromValueMixin<ValueWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::Value* value);
    static bool isInstance(Napi::Value value);
    llvm::Value *getValue();

protected:
    static Napi::FunctionReference& valueTemplate();

    explicit ValueWrapper(llvm::Value* value)
            : Napi::ObjectWrap {}, value { value } {
        assert(value && "value pointer is missing");
    }


private:
    llvm::Value* value;

    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value dump(const Napi::CallbackInfo& info);
    Napi::Value getType(const Napi::CallbackInfo& info);
    static Napi::Value hasName(const Napi::CallbackInfo& info);
    Napi::Value getName(const Napi::CallbackInfo& info);
    void setName(const Napi::CallbackInfo& info, const Napi::Value& value);
    static Napi::Value deleteValue(const Napi::CallbackInfo& info);
    static Napi::Value replaceAllUsesWith(const Napi::CallbackInfo& info);
    static Napi::Value useEmpty(const Napi::CallbackInfo& info);
};


#endif //LLVM_NODE_VALUE_H
