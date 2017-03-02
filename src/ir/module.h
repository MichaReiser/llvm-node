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
    std::unique_ptr<llvm::Module> module;

    ModuleWrapper(llvm::StringRef moduleId, llvm::LLVMContext& context)
    : module {llvm::make_unique<llvm::Module>(moduleId, context) } {
    }

    // static
    static NAN_METHOD(New);

    // instance
    static NAN_METHOD(dump);
    static NAN_METHOD(getFunction);
    static NAN_METHOD(setDataLayout);
    static NAN_METHOD(setSourceFileName);
    static NAN_METHOD(setTargetTriple);

    static inline Nan::Persistent<v8::FunctionTemplate>& moduleTemplate() {
        static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};
        return functionTemplate;
    }
};

#endif //LLVM_NODE_MODULE_H
