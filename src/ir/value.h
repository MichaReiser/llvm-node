//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_VALUE_H
#define LLVM_NODE_VALUE_H

#include <napi.h>
#include <llvm/IR/Value.h>

class ValueWrapper: public Napi::ObjectWrap<ValueWrapper> {
public:
    static void Init(Napi::Env env, Napi::Object& exports);
    static Napi::Value of(Napi::Env, llvm::Value* value);
    static bool isInstanceOfType(Napi::Value);
    llvm::Value *getValue();
    explicit ValueWrapper(const Napi::CallbackInfo& info);

private:
    static Napi::FunctionReference constructor;
    llvm::Value* value;

    void dump(const Napi::CallbackInfo& info);
    Napi::Value getType(const Napi::CallbackInfo& info);
    Napi::Value hasName(const Napi::CallbackInfo& info);
    Napi::Value getName(const Napi::CallbackInfo& info);
    void setName(const Napi::CallbackInfo& info, const Napi::Value& value);
    void deleteValue(const Napi::CallbackInfo& info);
    void replaceAllUsesWith(const Napi::CallbackInfo& info);
    Napi::Value useEmpty(const Napi::CallbackInfo& info);
};


#endif //LLVM_NODE_VALUE_H
