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
    Nan::SetPrototypeMethod(functionTemplate, "getFunction", getFunction);
    Nan::SetPrototypeMethod(functionTemplate, "setDataLayout", setDataLayout);
    Nan::SetPrototypeMethod(functionTemplate, "setSourceFileName", setSourceFileName);
    Nan::SetPrototypeMethod(functionTemplate, "setTargetTriple", setTargetTriple);
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

    auto* moduleWrapper = new ModuleWrapper { llvm::StringRef { moduleId.c_str(), moduleId.length() }, llvmContextWrapper->get() };
    moduleWrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ModuleWrapper::dump) {
    ModuleWrapper::FromValue(info.Holder())->module->dump();
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

NAN_METHOD(ModuleWrapper::setDataLayout) {
    if (info.Length() < 1 || !DataLayoutWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("setDataLayout needs to be called with a single argument, the data layout");
    }

    DataLayoutWrapper* dataLayoutWrapper = DataLayoutWrapper::FromValue(info[0]);
    auto wrapper = ModuleWrapper::FromValue(info.Holder());
    wrapper->module->setDataLayout(dataLayoutWrapper->get());
}

NAN_METHOD(ModuleWrapper::setSourceFileName) {
    if (info.Length() < 1 || !info[0]->IsString()) {
        return Nan::ThrowTypeError("setSourceFileName needs to be called with a single string argument (the source file name)");
    }

    Nan::Utf8String sourceFileName { Nan::To<v8::String>(info[0]).ToLocalChecked() };
    auto wrapper = ModuleWrapper::FromValue(info.Holder());

    wrapper->module->setSourceFileName(llvm::StringRef(*sourceFileName));
}

NAN_METHOD(ModuleWrapper::setTargetTriple) {
    if (info.Length() < 1 || !info[0]->IsString()) {
        return Nan::ThrowTypeError("setTargetTryple needs to be called with a single argument, the target triple (string)");
    }

    Nan::Utf8String triple { Nan::To<v8::String>(info[0]).ToLocalChecked() };
    auto wrapper = ModuleWrapper::FromValue(info.Holder());
    wrapper->module->setTargetTriple(llvm::StringRef(*triple));
}

llvm::Module *ModuleWrapper::getModule() {
    return module.get();
}

bool ModuleWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(moduleTemplate())->HasInstance(value);
}
