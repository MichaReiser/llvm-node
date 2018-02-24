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

Napi::Object ModuleWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::FunctionReference functionTemplate = Napi::Function::New(env, New);
    functionTemplate->SetClassName(Napi::String::New(env, "Module"));


#if LLVM_NODE_DEBUG
    InstanceMethod("dump", &dump),
#endif

    Napi::SetAccessor(functionTemplate->InstanceTemplate(), Napi::String::New(env, "empty"), empty);
    InstanceMethod("getFunction", &getFunction),
    InstanceMethod("getOrInsertFunction", &getOrInsertFunction),
    InstanceMethod("getGlobalVariable", &getGlobalVariable),
    Napi::SetAccessor(functionTemplate->InstanceTemplate(), Napi::String::New(env, "name"), getName);
    Napi::SetAccessor(functionTemplate->InstanceTemplate(), Napi::String::New(env, "dataLayout"), getDataLayout, setDataLayout);
    Napi::SetAccessor(functionTemplate->InstanceTemplate(), Napi::String::New(env, "moduleIdentifier"), getModuleIdentifier, setModuleIdentifier);
    Napi::SetAccessor(functionTemplate->InstanceTemplate(), Napi::String::New(env, "sourceFileName"), getSourceFileName, setSourceFileName);
    Napi::SetAccessor(functionTemplate->InstanceTemplate(), Napi::String::New(env, "targetTriple"), getTargetTriple, setTargetTriple);
    Napi::SetPrototypeMethod(functionTemplate, "print", ModuleWrapper::print);
    moduleTemplate().Reset(functionTemplate);

    auto constructorFunction = Napi::GetFunction(functionTemplate);
    (target).Set(Napi::String::New(env, "Module"), constructorFunction);
}

Napi::Value ModuleWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "The Module functionTemplate needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() < 2 || !info[0].IsString() || !LLVMContextWrapper::isInstance(info[1])) {
        Napi::TypeError::New(env, "The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto moduleId = ToString(info[0].ToString());
    auto* llvmContextWrapper = LLVMContextWrapper::FromValue(info[1]);

    auto* moduleWrapper = new ModuleWrapper { llvm::StringRef { moduleId.c_str(), moduleId.Length() },
                                              llvmContextWrapper->getContext() };
    moduleWrapper->Wrap(info.This());

    return info.This();
}

#if LLVM_NODE_DEBUG
Napi::Value ModuleWrapper::dump(const Napi::CallbackInfo& info) {
    ModuleWrapper::FromValue(info.Holder())->module->dump();
}
#endif

Napi::Value ModuleWrapper::empty(const Napi::CallbackInfo& info) {
    bool empty = ModuleWrapper::FromValue(info.Holder())->module->empty();
    return Napi::New(env, empty);
}

Napi::Value ModuleWrapper::getFunction(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "getFunction needs to be called with: name: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string functionName = ToString(info[0].To<Napi::String>());
    auto* wrapper = ModuleWrapper::FromValue(info.Holder());
    auto* function = wrapper->module->getFunction(functionName);

    if (function) {
        return return FunctionWrapper::of(function);
    }

    return env.Undefined();
}

Napi::Value ModuleWrapper::getOrInsertFunction(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !info[0].IsString() || !FunctionTypeWrapper::isInstance(info[1])) {
        Napi::TypeError::New(env, "getOrInsertFunction needs to be called with: name: string, functionType: FunctionType").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    std::string name = ToString(info[0]);
    auto* fnType = FunctionTypeWrapper::FromValue(info[1])->getFunctionType();

    return ConstantWrapper::of(module->getOrInsertFunction(name, fnType));
}

Napi::Value ModuleWrapper::getGlobalVariable(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !info[0].IsString() ||
            (info.Length() == 2 && !info[1].IsBoolean()) ||
            info.Length() > 2) {
        Napi::TypeError::New(env, "getGlobalVariable needs to be called with: name: string, allowInternal?: boolean").ThrowAsJavaScriptException();
        return env.Null();
    }

    const auto name = ToString(info[0]);
    const auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    bool allowInternal = false;

    if (info.Length() == 2) {
        allowInternal = info[1].As<Napi::Boolean>().Value();
    }

    auto* global = module->getGlobalVariable(name, allowInternal);
    if (global != nullptr) {
        return GlobalVariableWrapper::of(global);
    }
}

Napi::Value ModuleWrapper::getName(const Napi::CallbackInfo& info) {
    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    auto name = Napi::New(env, module->getName().str());
    return name;
}

Napi::Value ModuleWrapper::getDataLayout(const Napi::CallbackInfo& info) {
    auto dataLayout = ModuleWrapper::FromValue(info.Holder())->getModule()->getDataLayout();
    return DataLayoutWrapper::of(dataLayout);
}

void ModuleWrapper::setDataLayout(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!DataLayoutWrapper::isInstance(value)) {
        Napi::TypeError::New(env, "dataLayout needs to an instance of DataLayout").ThrowAsJavaScriptException();
        return env.Null();
    }

    DataLayoutWrapper* dataLayoutWrapper = DataLayoutWrapper::FromValue(value);
    auto wrapper = ModuleWrapper::FromValue(info.Holder());
    wrapper->module->setDataLayout(dataLayoutWrapper->getDataLayout());
}

Napi::Value ModuleWrapper::getModuleIdentifier(const Napi::CallbackInfo& info) {
    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    auto identifier = Napi::New(env, module->getModuleIdentifier());
    return identifier;
}

void ModuleWrapper::setModuleIdentifier(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!value.IsString()) {
        Napi::TypeError::New(env, "moduleIdentifier needs to be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string moduleIdentifier{ value.To<Napi::String>() };
    auto wrapper = ModuleWrapper::FromValue(info.Holder());

    wrapper->module->setModuleIdentifier(llvm::StringRef(*moduleIdentifier));
}

Napi::Value ModuleWrapper::getSourceFileName(const Napi::CallbackInfo& info) {
    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    auto name = Napi::New(env, module->getSourceFileName());
    return name;
}

void ModuleWrapper::setSourceFileName(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!value.IsString()) {
        Napi::TypeError::New(env, "sourceFileName needs to be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string sourceFileName { value.To<Napi::String>() };
    auto wrapper = ModuleWrapper::FromValue(info.Holder());

    wrapper->module->setSourceFileName(llvm::StringRef(*sourceFileName));
}

Napi::Value ModuleWrapper::getTargetTriple(const Napi::CallbackInfo& info) {
    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    auto name = Napi::New(env, module->getTargetTriple());
    return name;
}

void ModuleWrapper::setTargetTriple(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!value.IsString()) {
        Napi::TypeError::New(env, "targetTriple needs to be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string triple { value.To<Napi::String>() };
    auto wrapper = ModuleWrapper::FromValue(info.Holder());
    wrapper->module->setTargetTriple(llvm::StringRef(*triple));
}

Napi::Value ModuleWrapper::print(const Napi::CallbackInfo& info) {
    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    std::string str {};
    llvm::raw_string_ostream output { str };

    module->print(output, nullptr);
    output.flush();

    return Napi::New(env, str);
}

llvm::Module *ModuleWrapper::getModule() {
    return module;
}

bool ModuleWrapper::isInstance(Napi::Value value) {
    Napi::Env env = value.Env();
    return Napi::New(env, moduleTemplate())->HasInstance(value);
}
