//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_MODULE_H
#define LLVM_NODE_MODULE_H

#include <napi.h>
#include <llvm/IR/Module.h>

class ModuleWrapper: public Napi::ObjectWrap<ModuleWrapper> {
public:

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static bool isInstanceOfType(const Napi::Value& value);
    ModuleWrapper(const Napi::CallbackInfo& info);
    llvm::Module* getModule();

private:
    llvm::Module* module;

    static Napi::FunctionReference constructor;

    Napi::Value dump(const Napi::CallbackInfo& info);
    Napi::Value empty(const Napi::CallbackInfo& info);
    Napi::Value name(const Napi::CallbackInfo& info);
//    Napi::Value getFunction(const Napi::CallbackInfo& info);
//    Napi::Value getDataLayout(const Napi::CallbackInfo& info);
//    void setDataLayout(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value getModuleIdentifier(const Napi::CallbackInfo& info);
    void setModuleIdentifier(const Napi::CallbackInfo& info, const Napi::Value& value);
//    Napi::Value getOrInsertFunction(const Napi::CallbackInfo& info);
//    Napi::Value getGlobalVariable(const Napi::CallbackInfo& info);
    void setSourceFileName(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value getSourceFileName(const Napi::CallbackInfo& info);
    Napi::Value getTargetTriple(const Napi::CallbackInfo& info);
    void setTargetTriple(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value print(const Napi::CallbackInfo& info);

};

#endif //LLVM_NODE_MODULE_H
