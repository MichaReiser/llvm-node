//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_MODULE_H
#define LLVM_NODE_MODULE_H

#include <napi.h>
#include <llvm/IR/Module.h>

class ModuleWrapper: public Napi::ObjectWrap<ModuleWrapper> {
public:
    static void Init(Napi::Env env, Napi::Object& exports);
    static bool isInstanceOfType(const Napi::Value& value);

    explicit ModuleWrapper(const Napi::CallbackInfo& info);
    llvm::Module* getModule();

private:
    static Napi::FunctionReference constructor;

    llvm::Module* module;

    void dump(const Napi::CallbackInfo& info);
    Napi::Value empty(const Napi::CallbackInfo& info);
    Napi::Value getFunction(const Napi::CallbackInfo& info);
    Napi::Value getName(const Napi::CallbackInfo& info);
    Napi::Value getDataLayout(const Napi::CallbackInfo& info);
    void setDataLayout(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value getModuleIdentifier(const Napi::CallbackInfo& info);
    void setModuleIdentifier(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value getOrInsertFunction(const Napi::CallbackInfo& info);
    Napi::Value getGlobalVariable(const Napi::CallbackInfo& info);
    Napi::Value getTypeByName(const Napi::CallbackInfo& info);
    void setSourceFileName(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value getSourceFileName(const Napi::CallbackInfo& info);
    Napi::Value getTargetTriple(const Napi::CallbackInfo& info);
    void setTargetTriple(const Napi::CallbackInfo& info, const Napi::Value& value);
    void print(const Napi::CallbackInfo& info);
};

#endif //LLVM_NODE_MODULE_H
