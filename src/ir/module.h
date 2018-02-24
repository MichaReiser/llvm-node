//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_API_MODULE_H
#define LLVM_NODE_API_MODULE_H

#include <napi.h>
#include <uv.h>
#include <llvm/IR/Module.h>
#include "../util/from-value-mixin.h"

class ModuleWrapper : public Napi::ObjectWrap<ModuleWrapper>, public FromValueMixin<ModuleWrapper> {
public:

    static Napi::Object Init(Napi::Env env, Napi::Object exports);

    llvm::Module* getModule();

    static bool isInstance(Napi::Value value);

private:
    llvm::Module* module;

    ModuleWrapper(llvm::StringRef moduleId, llvm::LLVMContext& context)
    : module { new llvm::Module { moduleId, context } } {
    }

    // static
    static Napi::Value New(const Napi::CallbackInfo& info);

    // instance
    static Napi::Value dump(const Napi::CallbackInfo& info);
    Napi::Value empty(const Napi::CallbackInfo& info);
    static Napi::Value getFunction(const Napi::CallbackInfo& info);
    Napi::Value getName(const Napi::CallbackInfo& info);
    Napi::Value getDataLayout(const Napi::CallbackInfo& info);
    void setDataLayout(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value getModuleIdentifier(const Napi::CallbackInfo& info);
    void setModuleIdentifier(const Napi::CallbackInfo& info, const Napi::Value& value);
    static Napi::Value getOrInsertFunction(const Napi::CallbackInfo& info);
    static Napi::Value getGlobalVariable(const Napi::CallbackInfo& info);
    void setSourceFileName(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value getSourceFileName(const Napi::CallbackInfo& info);
    Napi::Value getTargetTriple(const Napi::CallbackInfo& info);
    void setTargetTriple(const Napi::CallbackInfo& info, const Napi::Value& value);
    static Napi::Value print(const Napi::CallbackInfo& info);

    static inline Napi::FunctionReference& moduleTemplate() {
        static Napi::FunctionReference functionTemplate {};
        return functionTemplate;
    }
};

#endif //LLVM_NODE_API_MODULE_H
