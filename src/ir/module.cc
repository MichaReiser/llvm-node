//
// Created by Micha Reiser on 28.02.17.
//

#include "module.h"
#include "../util/string.h"
#include "./llvm-context.h"
#include "./data-layout.h"
#include "function.h"

NAN_MODULE_INIT(ModuleWrapper::Init) {
    v8::Local<v8::FunctionTemplate> functionTemplate = Nan::New<v8::FunctionTemplate>(New);
    functionTemplate->SetClassName(Nan::New("Module").ToLocalChecked());
    functionTemplate->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(functionTemplate, "dump", dump);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("empty").ToLocalChecked(), empty);
    Nan::SetPrototypeMethod(functionTemplate, "getFunction", getFunction);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("name").ToLocalChecked(), getName);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("dataLayout").ToLocalChecked(), getDataLayout, setDataLayout);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("moduleIdentifier").ToLocalChecked(), getModuleIdentifier, setModuleIdentifier);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("sourceFileName").ToLocalChecked(), getSourceFileName, setSourceFileName);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("targetTriple").ToLocalChecked(), getTargetTriple, setTargetTriple);
    moduleTemplate().Reset(functionTemplate);

    auto constructorFunction = Nan::GetFunction(functionTemplate).ToLocalChecked();
    Nan::Set(target, Nan::New("Module").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(ModuleWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("The Module functionTemplate needs to be called with new");
    }

    if (info.Length() < 2 || !info[0]->IsString() || !LLVMContextWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)");
    }

    auto moduleId = ToString(info[0]->ToString());
    auto* llvmContextWrapper = LLVMContextWrapper::FromValue(info[1]);

    auto* moduleWrapper = new ModuleWrapper { llvm::StringRef { moduleId.c_str(), moduleId.length() },
                                              llvmContextWrapper->getContext() };
    moduleWrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ModuleWrapper::dump) {
    ModuleWrapper::FromValue(info.Holder())->module->dump();
}

NAN_GETTER(ModuleWrapper::empty) {
    bool empty = ModuleWrapper::FromValue(info.Holder())->module->empty();
    info.GetReturnValue().Set(Nan::New(empty));
}

NAN_METHOD(ModuleWrapper::getFunction) {
    if (info.Length() != 1 || !info[0]->IsString()) {
        return Nan::ThrowTypeError("getFunction needs to be called with: name: string");
    }

    std::string functionName = ToString(Nan::To<v8::String>(info[0]).ToLocalChecked());
    auto* wrapper = ModuleWrapper::FromValue(info.Holder());
    auto* function = wrapper->module->getFunction(functionName);

    if (function) {
        return info.GetReturnValue().Set(FunctionWrapper::of(function));
    }

    info.GetReturnValue().Set(Nan::Undefined());
}

NAN_GETTER(ModuleWrapper::getName) {
    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    auto name = Nan::New(module->getName().str()).ToLocalChecked();
    info.GetReturnValue().Set(name);
}

NAN_GETTER(ModuleWrapper::getDataLayout) {
    auto dataLayout = ModuleWrapper::FromValue(info.Holder())->getModule()->getDataLayout();
    info.GetReturnValue().Set(DataLayoutWrapper::of(dataLayout));
}

NAN_SETTER(ModuleWrapper::setDataLayout) {
    if (!DataLayoutWrapper::isInstance(value)) {
        return Nan::ThrowTypeError("dataLayout needs to an instance of DataLayout");
    }

    DataLayoutWrapper* dataLayoutWrapper = DataLayoutWrapper::FromValue(value);
    auto wrapper = ModuleWrapper::FromValue(info.Holder());
    wrapper->module->setDataLayout(dataLayoutWrapper->getDataLayout());
}

NAN_GETTER(ModuleWrapper::getModuleIdentifier) {
    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    auto identifier = Nan::New(module->getModuleIdentifier()).ToLocalChecked();
    info.GetReturnValue().Set(identifier);
}

NAN_SETTER(ModuleWrapper::setModuleIdentifier) {
    if (!value->IsString()) {
        return Nan::ThrowTypeError("moduleIdentifier needs to be a string");
    }

    Nan::Utf8String moduleIdentifier{ Nan::To<v8::String>(value).ToLocalChecked() };
    auto wrapper = ModuleWrapper::FromValue(info.Holder());

    wrapper->module->setModuleIdentifier(llvm::StringRef(*moduleIdentifier));
}

NAN_GETTER(ModuleWrapper::getSourceFileName) {
    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    auto name = Nan::New(module->getSourceFileName()).ToLocalChecked();
    info.GetReturnValue().Set(name);
}

NAN_SETTER(ModuleWrapper::setSourceFileName) {
    if (!value->IsString()) {
        return Nan::ThrowTypeError("sourceFileName needs to be a string");
    }

    Nan::Utf8String sourceFileName { Nan::To<v8::String>(value).ToLocalChecked() };
    auto wrapper = ModuleWrapper::FromValue(info.Holder());

    wrapper->module->setSourceFileName(llvm::StringRef(*sourceFileName));
}

NAN_GETTER(ModuleWrapper::getTargetTriple) {
    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    auto name = Nan::New(module->getTargetTriple()).ToLocalChecked();
    info.GetReturnValue().Set(name);
}

NAN_SETTER(ModuleWrapper::setTargetTriple) {
    if (!value->IsString()) {
        return Nan::ThrowTypeError("targetTriple needs to be a string");
    }

    Nan::Utf8String triple { Nan::To<v8::String>(value).ToLocalChecked() };
    auto wrapper = ModuleWrapper::FromValue(info.Holder());
    wrapper->module->setTargetTriple(llvm::StringRef(*triple));
}

llvm::Module *ModuleWrapper::getModule() {
    return module;
}

bool ModuleWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(moduleTemplate())->HasInstance(value);
}
