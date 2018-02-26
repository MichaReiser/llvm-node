//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_MODULE_H
#define LLVM_NODE_MODULE_H

#include <nan.h>
#include <llvm/IR/Module.h>
#include "../util/from-value-mixin.h"

class ModuleWrapper: public Nan::ObjectWrap, public FromValueMixin<ModuleWrapper> {
public:

    static NAN_MODULE_INIT(Init);

    llvm::Module* getModule();

    static bool isInstance(v8::Local<v8::Value> value);

private:
    llvm::Module* module;

    ModuleWrapper(llvm::StringRef moduleId, llvm::LLVMContext& context)
    : module { new llvm::Module { moduleId, context } } {
    }

    // static
    static NAN_METHOD(New);

    // instance
    static NAN_METHOD(dump);
    static NAN_GETTER(empty);
    static NAN_METHOD(getFunction);
    static NAN_GETTER(getName);
    static NAN_GETTER(getDataLayout);
    static NAN_SETTER(setDataLayout);
    static NAN_GETTER(getModuleIdentifier);
    static NAN_SETTER(setModuleIdentifier);
    static NAN_METHOD(getOrInsertFunction);
    static NAN_METHOD(getGlobalVariable);
    static NAN_METHOD(getTypeByName);
    static NAN_SETTER(setSourceFileName);
    static NAN_GETTER(getSourceFileName);
    static NAN_GETTER(getTargetTriple);
    static NAN_SETTER(setTargetTriple);
    static NAN_METHOD(print);

    static inline Nan::Persistent<v8::FunctionTemplate>& moduleTemplate() {
        static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};
        return functionTemplate;
    }
};

#endif //LLVM_NODE_MODULE_H
