//
// Created by Micha Reiser on 28.02.17.
//

#include <llvm/Support/raw_ostream.h>
#include "module.h"
//#include "../util/string.h"
#include "./llvm-context.h"
//#include "./data-layout.h"
//#include "function.h"
//#include "function-type.h"
//#include "global-variable.h"

Napi::FunctionReference ModuleWrapper::constructor;

Napi::Object ModuleWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope{env};

    Napi::Function func = DefineClass(env, "Module", {
            InstanceAccessor("empty", &ModuleWrapper::empty, nullptr),
//            InstanceMethod("getFunction", &ModuleWrapper::getFunction),
//            InstanceMethod("getOrInsertFunction", &ModuleWrapper::getOrInsertFunction),
//            InstanceMethod("getGlobalVariable", &ModuleWrapper::getGlobalVariable),
            InstanceAccessor("name", &ModuleWrapper::name, nullptr),
//            InstanceAccessor("dataLayout", &ModuleWrapper::getDataLayout, &ModuleWrapper::setDataLayout),
            InstanceAccessor("moduleIdentifier", &ModuleWrapper::getModuleIdentifier,
                             &ModuleWrapper::setModuleIdentifier),
            InstanceAccessor("sourceFileName", &ModuleWrapper::getSourceFileName, &ModuleWrapper::setSourceFileName),
            InstanceAccessor("targetTriple", &ModuleWrapper::getTargetTriple, &ModuleWrapper::setTargetTriple),
#if LLVM_NODE_DEBUG
            InstanceMethod("dump", &ModuleWrapper::dump);
#endif
            InstanceMethod("print", &ModuleWrapper::print)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Module", func);
    return exports;
}

ModuleWrapper::ModuleWrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<ModuleWrapper>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope{env};

    if (!info.IsConstructCall()) {
        throw Napi::TypeError::New(env, "The Module constructor needs to be called with new");
    }

    if (info.Length() < 2 || !info[0].IsString() || !LLVMContextWrapper::isInstanceOfType(info[1])) {
        throw Napi::TypeError::New(env,
                             "The Module constructor needs to be called with: (moduleId: string, context: LLVMContext)");
    }

    const std::string moduleId = info[0].As<Napi::String>();
    auto *llvmContextWrapper = LLVMContextWrapper::Unwrap(info[1].As<Napi::Object>());

    this->module = new llvm::Module(moduleId, llvmContextWrapper->getContext());
}

#if LLVM_NODE_DEBUG
Napi::Value ModuleWrapper::dump(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    this->module.dump();
    return env.Null();
}
#endif

Napi::Value ModuleWrapper::empty(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, this->module->empty());
}

//Napi::Value ModuleWrapper::getFunction(const Napi::CallbackInfo &info) {
//    Napi::Env env = info.Env();
//
//    if (info.Length() != 1 || !info[0].IsString()) {
//        throw Napi::TypeError::New(env, "getFunction needs to be called with: name: string");
//    }
//
//    const std::string functionName = info[0].As<Napi::String>;
//    auto *function = this->module->getFunction(functionName);
//
//    if (function) {
//        return FunctionWrapper::of(function);
//    }
//
//    return env.Null();
//}
//
//Napi::Value ModuleWrapper::getOrInsertFunction(const Napi::CallbackInfo &info) {
//    Napi::Env env = info.Env();
//    if (info.Length() != 2 || !info[0].IsString() || !FunctionTypeWrapper::isInstance(info[1])) {
//        throw Napi::TypeError::New(env,
//                             "getOrInsertFunction needs to be called with: name: string, functionType: FunctionType");
//    }
//
//    const std::string name = info[0].As<Napi::String>;
//    auto *fnType = FunctionTypeWrapper::FromValue(info[1])->getFunctionType();
//    return ConstantWrapper::of(this->module->getOrInsertFunction(name, fnType));
//}
//
//Napi::Value ModuleWrapper::getGlobalVariable(const Napi::CallbackInfo &info) {
//    Napi::Env env = info.Env();
//    if (info.Length() < 1 || !info[0].IsString() ||
//        (info.Length() == 2 && !info[1].IsBoolean()) ||
//        info.Length() > 2) {
//        throw Napi::TypeError::New(env,
//                             "getGlobalVariable needs to be called with: name: string, allowInternal?: boolean");
//    }
//
//    const std::string name = info[0].As<Napi::String>();
//    bool allowInternal = false;
//
//    if (info.Length() == 2) {
//        allowInternal = info[1].As<Napi::Boolean>();
//    }
//
//    auto *global = this->module->getGlobalVariable(name, allowInternal);
//    if (global == nullptr) {
//        return env.Null();
//    }
//
//    return GlobalVariableWrapper::of(global);
//}

Napi::Value ModuleWrapper::name(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->module->getName());
}

//Napi::Value ModuleWrapper::getDataLayout(const Napi::CallbackInfo &info) {
//    Napi::Env env = info.Env();
//
//    auto dataLayout = this->getModule()->getDataLayout();
//    return DataLayoutWrapper::of(dataLayout);
//}
//
//Napi::Value ModuleWrapper::setDataLayout(const Napi::CallbackInfo &info, const Napi::Value &value) {
//    Napi::Env env = info.Env();
//    if (!DataLayoutWrapper::isInstance(value)) {
//        throw Napi::TypeError::New(env, "dataLayout needs to an instance of DataLayout");
//    }
//
//    DataLayoutWrapper *dataLayoutWrapper = DataLayoutWrapper::FromValue(value);
//    this->module->setDataLayout(dataLayoutWrapper->getDataLayout());
//}

Napi::Value ModuleWrapper::getModuleIdentifier(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->module->getModuleIdentifier());
}

void ModuleWrapper::setModuleIdentifier(const Napi::CallbackInfo &info, const Napi::Value &value) {
    Napi::Env env = info.Env();
    if (!value.IsString()) {
        throw Napi::TypeError::New(env, "moduleIdentifier needs to be a string");
    }

    const std::string moduleIdentifier = value.As<Napi::String>();
    this->module->setModuleIdentifier(moduleIdentifier);
}

Napi::Value ModuleWrapper::getSourceFileName(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->module->getSourceFileName());
}

void ModuleWrapper::setSourceFileName(const Napi::CallbackInfo &info, const Napi::Value &value) {
    Napi::Env env = info.Env();
    if (!value.IsString()) {
        throw Napi::TypeError::New(env, "sourceFileName needs to be a string");
    }

    const std::string sourceFileName = value.As<Napi::String>();
    this->module->setSourceFileName(sourceFileName);
}

Napi::Value ModuleWrapper::getTargetTriple(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, this->module->getTargetTriple());
}

void ModuleWrapper::setTargetTriple(const Napi::CallbackInfo &info, const Napi::Value &value) {
    Napi::Env env = info.Env();
    if (!value.IsString()) {
        throw Napi::TypeError::New(env, "targetTriple needs to be a string");
    }

    const std::string triple = value.As<Napi::String>();
    this->module->setTargetTriple(triple);
}

Napi::Value ModuleWrapper::print(const Napi::CallbackInfo &info) {
    std::string str{};
    llvm::raw_string_ostream output{str};

    this->module->print(output, nullptr);
    output.flush();

    return Napi::String::New(info.Env(), str);
}

llvm::Module *ModuleWrapper::getModule() {
    return module;
}

bool ModuleWrapper::isInstanceOfType(const Napi::Value &value) {
    return value.As<Napi::Object>().InstanceOf(constructor.Value());
}