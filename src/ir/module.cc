//
// Created by Micha Reiser on 28.02.17.
//

#include <llvm/Support/raw_ostream.h>
#include "module.h"
#include "../util/string.h"
#include "./llvm-context.h"
#include "./data-layout.h"
#include "function.h"
#include "function-type.h"
#include "global-variable.h"
#include "struct-type.h"

Napi::FunctionReference ModuleWrapper::constructor;

void ModuleWrapper::Init(Napi::Env env, Napi::Object &exports) {
    Napi::HandleScope scope { env };

    Napi::Function func = DefineClass(env, "Module", {
        InstanceAccessor("empty", &ModuleWrapper::empty, nullptr),
        InstanceMethod("getFunction", &ModuleWrapper::getFunction),
        InstanceMethod("getOrInsertFunction", &ModuleWrapper::getOrInsertFunction),
        InstanceMethod("getGlobalVariable", &ModuleWrapper::getGlobalVariable),
        InstanceMethod("getTypeByName", &ModuleWrapper::getTypeByName),
        InstanceAccessor("name", &ModuleWrapper::getName, nullptr),
        InstanceAccessor("dataLayout", &ModuleWrapper::getDataLayout, &ModuleWrapper::setDataLayout),
        InstanceAccessor("moduleIdentifier", &ModuleWrapper::getModuleIdentifier, &ModuleWrapper::setModuleIdentifier),
        InstanceAccessor("sourceFileName", &ModuleWrapper::getSourceFileName, &ModuleWrapper::setSourceFileName),
        InstanceAccessor("targetTriple", &ModuleWrapper::getTargetTriple, &ModuleWrapper::setTargetTriple),
        InstanceMethod("print", &ModuleWrapper::print)
#if LLVM_NODE_DEBUG
        , InstanceMethod("dump", &ModuleWrapper::dump)
#endif
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Module", func);
}

bool ModuleWrapper::isInstanceOfType(const Napi::Value &value) {
    return value.As<Napi::Object>().InstanceOf(constructor.Value());
}

ModuleWrapper::ModuleWrapper(const Napi::CallbackInfo &info): Napi::ObjectWrap<ModuleWrapper> { info } {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    if (!info.IsConstructCall()) {
        throw Napi::TypeError::New(env, "Module constructor needs to be called with new");
    }

    if (info.Length() < 2 || !info[0].IsString() || !LLVMContextWrapper::isInstanceOfType(info[1])) {
        throw Napi::TypeError::New(env, "The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)");
    }

    std::string moduleId = info[0].As<Napi::String>();
    auto* llvmContextWrapper = LLVMContextWrapper::Unwrap(info[1].As<Napi::Object>());
    module = new llvm::Module(llvm::StringRef { moduleId }, llvmContextWrapper->getContext());
}

#if LLVM_NODE_DEBUG
void ModuleWrapper::dump(const Napi::CallbackInfo& info) {
    module->dump();
}
#endif

Napi::Value ModuleWrapper::empty(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    return Napi::Boolean::New(env, module->empty());
}

Napi::Value ModuleWrapper::getFunction(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    if (info.Length() != 1 || !info[0].IsString()) {
        throw Napi::TypeError::New(env, "getFunction needs to be called with: name: string");
    }

    std::string functionName = info[0].As<Napi::String>();
    auto* function = module->getFunction(functionName);

    // TODO uncomment when function is migrated
//    if (function) {
//        return FunctionWrapper::of(function);
//    }

    return env.Undefined();
}

Napi::Value ModuleWrapper::getOrInsertFunction(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };


//    TODO uncomment when FunctionType and Constant is migrated
//    if (info.Length() != 2 || !info[0].IsString() || !FunctionTypeWrapper::isInstance(info[1])) {
//        throw Napi::TypeError::New(env, "getOrInsertFunction needs to be called with: name: string, functionType: FunctionType");
//    }
//    std::string name = info[0].As<Napi::String>();
//    auto* fnType = FunctionTypeWrapper::FromValue(info[1])->getFunctionType();
//    return ConstantWrapper::of(module->getOrInsertFunction(name, fnType);

    return env.Undefined();
}

Napi::Value ModuleWrapper::getGlobalVariable(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    if (info.Length() < 1 || !info[0].IsString() ||
        (info.Length() == 2 && !info[1].IsBoolean()) ||
        info.Length() > 2) {
        throw Napi::TypeError::New(env, "getGlobalVariable needs to be called with: name: string, allowInternal?: boolean");
    }

    const std::string name = info[0].As<Napi::String>();
    bool allowInternal = false;

    if (info.Length() == 2) {
        allowInternal = info[1].As<Napi::Boolean>();
    }

    auto* global = module->getGlobalVariable(name, allowInternal);
    if (global != nullptr) {
//        TODO uncomment when global variable is migrated
//        return GlobalVariableWrapper::of(global);
    }

    return env.Undefined();
}

Napi::Value ModuleWrapper::getTypeByName(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    if (info.Length() < 1 || !info[0].IsString()) {
        throw Napi::TypeError::New(env, "getTypeByName needs to be called with: name: string");
    }

    const std::string name = info[0].As<Napi::String>();
    llvm::StructType* type = module->getTypeByName(name);

    if (type == nullptr) {
        return env.Null();
    }

//    TODO uncomment when struct type is migrated
//    return StructTypeWrapper::of(type);
}

Napi::Value ModuleWrapper::getName(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    return Napi::String::New(env, module->getName());
}

Napi::Value ModuleWrapper::getDataLayout(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };
    auto dataLayout = module->getDataLayout();
//    TODO uncomment when data layout has been migrated
//    return DataLayoutWrapper::of(dataLayout);

    return env.Undefined();
}

void ModuleWrapper::setDataLayout(const Napi::CallbackInfo &info, const Napi::Value &value) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

//    TODO uncomment when data layout wrapper has been migrated
//    if (!DataLayoutWrapper::isInstance(value)) {
//        throw Napi::TypeError::new(env, "dataLayout needs to an instance of DataLayout");
//    }
//
//    DataLayoutWrapper* dataLayoutWrapper = DataLayoutWrapper::Unwrap(value);
//    module->setDataLayout(dataLayoutWrapper->getDataLayout());
}

Napi::Value ModuleWrapper::getModuleIdentifier(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    return Napi::String::New(env, module->getModuleIdentifier());
}

void ModuleWrapper::setModuleIdentifier(const Napi::CallbackInfo &info, const Napi::Value &value) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    if (!value.IsString()) {
        throw Napi::TypeError::New(env, "moduleIdentifier needs to be a string");
    }

    std::string moduleIdentifier = value.As<Napi::String>();
    module->setModuleIdentifier(llvm::StringRef(moduleIdentifier));
}

Napi::Value ModuleWrapper::getSourceFileName(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    return Napi::String::New(env, module->getSourceFileName());
}

void ModuleWrapper::setSourceFileName(const Napi::CallbackInfo &info, const Napi::Value &value) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    if (!value.IsString()) {
        throw Napi::TypeError::New(env, "sourceFileName needs to be a string");
    }

    std::string sourceFileName = value.As<Napi::String>();
    module->setSourceFileName(llvm::StringRef(sourceFileName));
}

Napi::Value ModuleWrapper::getTargetTriple(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    return Napi::String::New(env, module->getTargetTriple());
}

void ModuleWrapper::setTargetTriple(const Napi::CallbackInfo &info, const Napi::Value &value) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    if (!value.IsString()) {
        throw Napi::TypeError::New(env, "targetTriple needs to be a string");
    }

    std::string triple = value.As<Napi::String>();
    module->setTargetTriple(llvm::StringRef(triple));
}

void ModuleWrapper::print(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    Napi::HandleScope scope { env };

    std::string str {};
    llvm::raw_string_ostream output { str };

    module->print(output, nullptr);
    output.flush();

    Napi::String::New(env, str);
}

llvm::Module *ModuleWrapper::getModule() {
    return module;
}