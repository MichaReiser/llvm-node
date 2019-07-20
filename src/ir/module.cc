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

NAN_MODULE_INIT(ModuleWrapper::Init) {
    v8::Local<v8::FunctionTemplate> functionTemplate = Nan::New<v8::FunctionTemplate>(New);
    functionTemplate->SetClassName(Nan::New("Module").ToLocalChecked());
    functionTemplate->InstanceTemplate()->SetInternalFieldCount(1);

#if LLVM_NODE_DEBUG
    Nan::SetPrototypeMethod(functionTemplate, "dump", dump);
#endif

    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("empty").ToLocalChecked(), empty);
    Nan::SetPrototypeMethod(functionTemplate, "getFunction", getFunction);
    Nan::SetPrototypeMethod(functionTemplate, "getOrInsertFunction", getOrInsertFunction);
    Nan::SetPrototypeMethod(functionTemplate, "getGlobalVariable", getGlobalVariable);
    Nan::SetPrototypeMethod(functionTemplate, "getTypeByName", getTypeByName);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("name").ToLocalChecked(), getName);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("dataLayout").ToLocalChecked(), getDataLayout, setDataLayout);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("moduleIdentifier").ToLocalChecked(), getModuleIdentifier, setModuleIdentifier);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("sourceFileName").ToLocalChecked(), getSourceFileName, setSourceFileName);
    Nan::SetAccessor(functionTemplate->InstanceTemplate(), Nan::New("targetTriple").ToLocalChecked(), getTargetTriple, setTargetTriple);
    Nan::SetPrototypeMethod(functionTemplate, "print", ModuleWrapper::print);
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

    auto moduleId = ToString(info[0]);
    auto* llvmContextWrapper = LLVMContextWrapper::FromValue(info[1]);

    auto* moduleWrapper = new ModuleWrapper { llvm::StringRef { moduleId.c_str(), moduleId.length() },
                                              llvmContextWrapper->getContext() };
    moduleWrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

#if LLVM_NODE_DEBUG
NAN_METHOD(ModuleWrapper::dump) {
    ModuleWrapper::FromValue(info.Holder())->module->dump();
}
#endif

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

NAN_METHOD(ModuleWrapper::getOrInsertFunction) {
    if (info.Length() != 2 || !info[0]->IsString() || !FunctionTypeWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("getOrInsertFunction needs to be called with: name: string, functionType: FunctionType");
    }

    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    std::string name = ToString(info[0]);
    auto* fnType = FunctionTypeWrapper::FromValue(info[1])->getFunctionType();

    auto functionCallee = Nan::New<v8::Object>();

    #if LLVM_VERSION_MAJOR < 9
        Nan::Set(functionCallee, Nan::New("callee").ToLocalChecked(), ConstantWrapper::of(module->getOrInsertFunction(name, fnType)));
        Nan::Set(functionCallee, Nan::New("functionType").ToLocalChecked(), FunctionTypeWrapper::of(fnType));
    #else
        auto llvmCallee = module->getOrInsertFunction(name, fnType);
        Nan::Set(functionCallee, Nan::New("callee").ToLocalChecked(), ValueWrapper::of(llvmCallee.getCallee()));
        Nan::Set(functionCallee, Nan::New("functionType").ToLocalChecked(), FunctionTypeWrapper::of(llvmCallee.getFunctionType()));
    #endif

    Nan::EscapableHandleScope scope {};
    info.GetReturnValue().Set(scope.Escape(functionCallee));
}

NAN_METHOD(ModuleWrapper::getGlobalVariable) {
    if (info.Length() < 1 || !info[0]->IsString() ||
            (info.Length() == 2 && !info[1]->IsBoolean()) ||
            info.Length() > 2) {
        return Nan::ThrowTypeError("getGlobalVariable needs to be called with: name: string, allowInternal?: boolean");
    }

    const auto name = ToString(info[0]);
    const auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    bool allowInternal = false;

    if (info.Length() == 2) {
        allowInternal = Nan::To<bool>(info[1]).FromJust();
    }

    auto* global = module->getGlobalVariable(name, allowInternal);
    if (global != nullptr) {
        info.GetReturnValue().Set(GlobalVariableWrapper::of(global));
    }
}

NAN_METHOD(ModuleWrapper::getTypeByName) {
    if (info.Length() < 1 || !info[0]->IsString()) {
        return Nan::ThrowTypeError("getTypeByName needs to be called with: name: string");
    }

    const std::string name = ToString(info[0]);
    const llvm::Module* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    llvm::StructType* type = module->getTypeByName(name);

    if (type == nullptr) {
        info.GetReturnValue().Set(Nan::Null());
    } else {
        info.GetReturnValue().Set(StructTypeWrapper::of(type));
    }
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

NAN_METHOD(ModuleWrapper::print) {
    auto* module = ModuleWrapper::FromValue(info.Holder())->getModule();
    std::string str {};
    llvm::raw_string_ostream output { str };

    module->print(output, nullptr);
    output.flush();

    info.GetReturnValue().Set(Nan::New(str).ToLocalChecked());
}

llvm::Module *ModuleWrapper::getModule() {
    return module;
}

bool ModuleWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(moduleTemplate())->HasInstance(value);
}
