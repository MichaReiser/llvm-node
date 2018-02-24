//
// Created by Micha Reiser on 01.03.17.
//

#include "function.h"
#include "function-type.h"
#include "../util/string.h"
#include "module.h"
#include "argument.h"
#include "basic-block.h"

FunctionWrapper::FunctionWrapper(llvm::Function *function) : ConstantWrapper { function } {}

Napi::Object FunctionWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, functionTemplate()));
    (target).Set(Napi::String::New(env, "Function"), constructorFunction);
}

Napi::Value FunctionWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Class Constructor Function cannot be invoked without new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "External Function Pointer required").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto * function = static_cast<llvm::Function*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new FunctionWrapper { function };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::FunctionReference &FunctionWrapper::functionTemplate() {
    static Napi::FunctionReference tmpl {};

    if (tmpl.IsEmpty()) {
        Napi::FunctionReference localTemplate = Napi::Function::New(env, FunctionWrapper::New);
        Napi::FunctionReference valueTemplate = Napi::New(env, ConstantWrapper::constantTemplate());

        localTemplate->SetClassName(Napi::String::New(env, "Function"));

        localTemplate->Inherit(valueTemplate);

        Napi::SetMethod(localTemplate, "create", FunctionWrapper::Create);
        Napi::SetPrototypeMethod(localTemplate, "addAttribute", FunctionWrapper::addAttribute);
        Napi::SetPrototypeMethod(localTemplate, "addBasicBlock", FunctionWrapper::addBasicBlock);
        Napi::SetPrototypeMethod(localTemplate, "addDereferenceableAttr", FunctionWrapper::addDereferenceableAttr);
        Napi::SetPrototypeMethod(localTemplate, "addDereferenceableOrNullAttr", FunctionWrapper::addDereferenceableOrNullAttr);
        Napi::SetPrototypeMethod(localTemplate, "addFnAttr", FunctionWrapper::addFnAttr);
        Napi::SetPrototypeMethod(localTemplate, "getArguments", FunctionWrapper::getArguments);
        Napi::SetPrototypeMethod(localTemplate, "getEntryBlock", FunctionWrapper::getEntryBlock);
        Napi::SetPrototypeMethod(localTemplate, "getBasicBlocks", FunctionWrapper::getBasicBlocks);
        Napi::SetPrototypeMethod(localTemplate, "viewCFG", FunctionWrapper::viewCFG);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "callingConv"), FunctionWrapper::getCallingConv, FunctionWrapper::setCallingConv);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "visibility"), FunctionWrapper::getVisibility, FunctionWrapper::setVisibility);

        tmpl.Reset(localTemplate);
    }

    return tmpl;
}

Napi::Object FunctionWrapper::of(llvm::Function *function) {
    Napi::FunctionReference localTemplate = Napi::New(env, functionTemplate());
    Napi::Function constructor = Napi::GetFunction(localTemplate);

    Napi::Value args[1] = { Napi::External::New(env, function) };

    Napi::Object instance = Napi::NewInstance(constructor, 1, args );

    Napi::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

Napi::Value FunctionWrapper::Create(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !FunctionTypeWrapper::isInstance(info[0]) || !info[1].IsUint32()
        || (info.Length() > 2 && !(info[2].IsUndefined() || info[2].IsString()))
        || (info.Length() > 3 && !ModuleWrapper::isInstance(info[3]))) {
        Napi::TypeError::New(env, "Create needs to be called with: functionType: FunctionType, linkageTypes: uint32, name: string?, module?: Module").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* functionType = FunctionTypeWrapper::FromValue(info[0])->getFunctionType();
    auto linkageType = static_cast<llvm::GlobalValue::LinkageTypes>(info[1].As<Napi::Number>().Uint32Value());

    std::string name = {};
    llvm::Module* module = nullptr;

    if (info.Length() > 2 && !info[2].IsUndefined()) {
        name = ToString(info[2].To<Napi::String>());
    }

    if (info.Length() == 4) {
        module = ModuleWrapper::FromValue(info[3])->getModule();
    }

    auto* function = llvm::Function::Create(functionType, linkageType, name, module);
    auto wrapper = of(function);
    return wrapper;
}

Napi::Value FunctionWrapper::addBasicBlock(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !BasicBlockWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "addBasicBlock needs to be called with: block: BasicBlock").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* block = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();
    auto* wrapper = FunctionWrapper::FromValue(info.Holder());
    wrapper->getFunction()->getBasicBlockList().push_back(block);
}

Napi::Value FunctionWrapper::addFnAttr(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !info[0].IsUint32()) {
        Napi::TypeError::New(env, "addFnAttr needs to be called with: attribute: Attribute.AttrKind").ThrowAsJavaScriptException();
        return env.Null();
    }


    auto* function = FunctionWrapper::FromValue(info.Holder())->getFunction();
    auto attribute = static_cast<llvm::Attribute::AttrKind>(info[0].As<Napi::Number>().Uint32Value());

    function->addFnAttr(attribute);
}

Napi::Value FunctionWrapper::addAttribute(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !info[0].IsUint32() || !info[1].IsUint32()) {
        Napi::TypeError::New(env, "addAttr needs to be called with: i: uint32, attribute: Attribute.AttrKind").ThrowAsJavaScriptException();
        return env.Null();
    }


    auto* function = FunctionWrapper::FromValue(info.Holder())->getFunction();
    auto i = info[0].As<Napi::Number>().Uint32Value();
    auto attribute = static_cast<llvm::Attribute::AttrKind>(info[1].As<Napi::Number>().Uint32Value());

    function->addAttribute(i, attribute);
}

Napi::Value FunctionWrapper::addDereferenceableAttr(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !info[0].IsUint32() || !info[1].IsUint32()) {
        Napi::TypeError::New(env, "addDereferenceableAttr needs to be called with: argumentIndex: uint32, bytes: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* function = FunctionWrapper::FromValue(info.Holder())->getFunction();
    auto index = info[0].As<Napi::Number>().Uint32Value();
    auto bytes = info[1].As<Napi::Number>().Uint32Value();

    function->addDereferenceableAttr(index, bytes);
}

Napi::Value FunctionWrapper::addDereferenceableOrNullAttr(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !info[0].IsUint32() || !info[1].IsUint32()) {
        Napi::TypeError::New(env, "addDereferenceableOrNullAttr needs to be called with: argumentIndex: uint32, bytes: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* function = FunctionWrapper::FromValue(info.Holder())->getFunction();
    auto index = info[0].As<Napi::Number>().Uint32Value();
    auto bytes = info[1].As<Napi::Number>().Uint32Value();

    function->addDereferenceableOrNullAttr(index, bytes);
}

Napi::Value FunctionWrapper::getArguments(const Napi::CallbackInfo& info) {
    auto* wrapper = FunctionWrapper::FromValue(info.Holder());
    auto result = Napi::Array::New(env, wrapper->getFunction()->arg_size());

    uint32_t i = 0;
    for (auto &arg : wrapper->getFunction()->args()) {
        result.Set(i, ArgumentWrapper::of(&arg));
        ++i;
    }

    return result;
}

Napi::Value FunctionWrapper::getEntryBlock(const Napi::CallbackInfo& info) {
    auto* function = FunctionWrapper::FromValue(info.Holder())->getFunction();

    if (function->empty()) {
        return return env.Null();
    }

    auto& entryBlock = function->getEntryBlock();
    return BasicBlockWrapper::of(&entryBlock);
}

Napi::Value FunctionWrapper::getBasicBlocks(const Napi::CallbackInfo& info) {
    auto* function = FunctionWrapper::FromValue(info.Holder())->getFunction();
    auto result = Napi::Array::New(env, function->size());

    uint32_t i = 0;
    for (auto &arg : *function) {
        result.Set(i, BasicBlockWrapper::of(&arg));
        ++i;
    }

    return result;
}

Napi::Value FunctionWrapper::getCallingConv(const Napi::CallbackInfo& info) {
    auto callingConv = FunctionWrapper::FromValue(info.Holder())->getFunction()->getCallingConv();
    return Napi::New(env, callingConv);
}

void FunctionWrapper::setCallingConv(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!value->IsUint32()) {
        Napi::TypeError::New(env, "callingConv needs to be a value of llvm.CallingConv").ThrowAsJavaScriptException();
        return env.Null();
    }

    uint32_t callingConvention = value.As<Napi::Number>().Uint32Value();

    FunctionWrapper::FromValue(info.Holder())->getFunction()->setCallingConv(callingConvention);
}

Napi::Value FunctionWrapper::getVisibility(const Napi::CallbackInfo& info) {
    auto* function = FunctionWrapper::FromValue(info.Holder())->getFunction();
    auto visibility = function->getVisibility();

    return Napi::New(env, visibility);
}

void FunctionWrapper::setVisibility(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!value->IsUint32()) {
        Napi::TypeError::New(env, "visibility needs to be an uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* function = FunctionWrapper::FromValue(info.Holder())->getFunction();
    auto visibility = static_cast<llvm::GlobalValue::VisibilityTypes>(value.As<Napi::Number>().Uint32Value());

    function->setVisibility(visibility);
}

Napi::Value FunctionWrapper::viewCFG(const Napi::CallbackInfo& info) {
    FunctionWrapper::FromValue(info.Holder())->getFunction()->viewCFG();
}

llvm::Function *FunctionWrapper::getFunction() {
    return static_cast<llvm::Function*>(getValue());
}

bool FunctionWrapper::isInstance(Napi::Value value) {
    Napi::Env env = value.Env();
    return Napi::New(env, functionTemplate())->HasInstance(value);
}
