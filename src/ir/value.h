//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_VALUE_H
#define LLVM_NODE_VALUE_H

#include <napi.h>
#include <llvm/IR/Value.h>

class ValueWrapper: public Napi::ObjectWrap<ValueWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(Napi::Env env, llvm::Value* value);
    static bool isInstanceOfType(const Napi::Value& value);
    static Napi::FunctionReference constructor;
    ValueWrapper(const Napi::CallbackInfo& info);
    llvm::Value *getValue();

private:
    llvm::Value* value;

    Napi::Value dump(const Napi::CallbackInfo& info);
//    Napi::Object getType(const Napi::CallbackInfo& info);
    Napi::Value hasName(const Napi::CallbackInfo& info);
    Napi::Value getName(const Napi::CallbackInfo& info);
    void setName(const Napi::CallbackInfo& info, const Napi::Value& value);
    void deleteValue(const Napi::CallbackInfo& info);
    void replaceAllUsesWith(const Napi::CallbackInfo& info);
    Napi::Value useEmpty(const Napi::CallbackInfo& info);
};


#endif //LLVM_NODE_VALUE_H
