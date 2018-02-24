
#include <llvm/IR/GlobalValue.h>
#include "global-variable.h"
#include "module.h"
#include "type.h"
#include "../util/string.h"
#include "../util/from-value-mixin.h"


Napi::Object GlobalVariableWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto globalVariable = Napi::GetFunction(Napi::New(env, globalVariableTemplate()));
    (target).Set(Napi::String::New(env, "GlobalVariable"), globalVariable);
}

Napi::Object GlobalVariableWrapper::of(llvm::GlobalVariable* variable) {
    auto constructor = Napi::GetFunction(Napi::New(env, globalVariableTemplate()));
    Napi::Value args[1] = { Napi::New<v8::External> (variable) };

    auto instance = Napi::NewInstance(constructor, 1, args);
    Napi::EscapableHandleScope escapeHandleScope {};
    return escapeHandleScope.Escape(instance);
}

llvm::GlobalVariable* GlobalVariableWrapper::getGlobalVariable() {
    return static_cast<llvm::GlobalVariable*>(getValue());
}

Napi::Value GlobalVariableWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "The GlobalVariable needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() == 1 && info[0].IsExternal()) {
        NewFromExternal(info);
    } else if (info.Length() >= 4 && ModuleWrapper::isInstance(info[0]) && TypeWrapper::isInstance(info[1]) && info[2].IsBoolean() && info[3].IsUint32()
               && (info.Length() < 5 || ConstantWrapper::isInstance(info[4]) || info[4].IsUndefined())
               && (info.Length() < 6 || info[5].IsString())) {
        NewFromArguments(info);
    } else {
        Napi::TypeError::New(env, "The GlobalVariable constructor needs to be called with: module: Module, type: Type, isConstant: boolean, linkageType: LinkageTypes, initializer?: Constant, name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }
}


Napi::Value GlobalVariableWrapper::NewFromExternal(const Napi::CallbackInfo& info) {
    auto* globalVariable = static_cast<llvm::GlobalVariable*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new GlobalVariableWrapper { globalVariable };
    wrapper->Wrap(info.This());
    return info.This();
}

Napi::Value GlobalVariableWrapper::NewFromArguments(const Napi::CallbackInfo& info) {
    auto* module = ModuleWrapper::FromValue(info[0])->getModule();
    auto* type = TypeWrapper::FromValue(info[1])->getType();
    auto constant = info[2].As<Napi::Boolean>().Value();
    auto linkageType = static_cast<llvm::GlobalValue::LinkageTypes>(info[3].As<Napi::Number>().Uint32Value());
    llvm::Constant* initializer = nullptr;
    std::string name {};

    if (info.Length() > 4 && !info[4].IsUndefined()) {
        initializer = ConstantWrapper::FromValue(info[4])->getConstant();
    }

    if (info.Length() > 5) {
        name = ToString(info[5]);
    }

    auto* globalVariable = new llvm::GlobalVariable { *module, type, constant, linkageType, initializer, name };

    auto* wrapper = new GlobalVariableWrapper { globalVariable };
    wrapper->Wrap(info.This());
    return info.This();
}

Napi::Value GlobalVariableWrapper::hasGlobalUnnamedAddr(const Napi::CallbackInfo& info) {
    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    return Napi::New(env, globalVariable->hasGlobalUnnamedAddr());
}

Napi::Value GlobalVariableWrapper::setUnnamedAddr(const Napi::CallbackInfo& info) {
    if (!info[0].IsUint32()) {
        Napi::TypeError::New(env, "setUnnamedAddr needs to be called with: addr: GlobalValue.UnnamedAddr").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    auto value = info[0].As<Napi::Number>().Uint32Value();
    globalVariable->setUnnamedAddr(static_cast<llvm::GlobalValue::UnnamedAddr>(value));
}

Napi::Value GlobalVariableWrapper::getConstant(const Napi::CallbackInfo& info) {
    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    return Napi::New(env, globalVariable->isConstant());
}

void GlobalVariableWrapper::setConstant(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!value->IsBoolean()) {
        Napi::TypeError::New(env, "GlobalVariable.constant is of type boolean").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    globalVariable->setConstant(value.As<Napi::Boolean>().Value());
}

Napi::Value GlobalVariableWrapper::getInitializer(const Napi::CallbackInfo& info) {
    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    auto* initializer = globalVariable->getInitializer();

    if (initializer) {
        return ConstantWrapper::of(initializer);
    } else {
        return env.Undefined();
    }
}

void GlobalVariableWrapper::setInitializer(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!value->IsUndefined() && !ConstantWrapper::isInstance(value)) {
        Napi::TypeError::New(env, "GlobalVariable.initializer is of type Constant").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    llvm::Constant* initializer = nullptr;

    if (!value->IsUndefined()) {
        initializer = ConstantWrapper::FromValue(value)->getConstant();
    }

    globalVariable->setInitializer(initializer);
}

Napi::FunctionReference& GlobalVariableWrapper::globalVariableTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        Napi::FunctionReference localTemplate = Napi::Function::New(env, GlobalVariableWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "GlobalVariable"));

        localTemplate->Inherit(Napi::New(env, constantTemplate()));

        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "constant"), GlobalVariableWrapper::getConstant, GlobalVariableWrapper::setConstant);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "initializer"), GlobalVariableWrapper::getInitializer, GlobalVariableWrapper::setInitializer);

        Napi::SetPrototypeMethod(localTemplate, "hasGlobalUnnamedAddr", GlobalVariableWrapper::hasGlobalUnnamedAddr);
        Napi::SetPrototypeMethod(localTemplate, "setUnnamedAddr", GlobalVariableWrapper::setUnnamedAddr);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}