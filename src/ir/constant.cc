//
// Created by Micha Reiser on 01.03.17.
//

#include <llvm/IR/Function.h>
#include "constant.h"
#include "type.h"
#include "function.h"
#include "pointer-type.h"
#include "../util/array.h"
#include "../util/string.h"
#include "constant-fp.h"
#include "constant-int.h"
#include "constant-data-array.h"
#include "constant-pointer-null.h"
#include "global-variable.h"
#include "constant-struct.h"
#include "constant-array.h"
#include "undef-value.h"

Napi::Object ConstantWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto constant = Napi::GetFunction(Napi::New(env, constantTemplate()));
    (target).Set(Napi::String::New(env, "Constant"), constant);
}

Napi::Object ConstantWrapper::of(llvm::Constant *constant) {
    Napi::Object result;

    if (llvm::Function::classof(constant)) {
        result = FunctionWrapper::of(static_cast<llvm::Function*>(constant));
    } else if (llvm::ConstantFP::classof(constant)) {
        result = ConstantFPWrapper::of(static_cast<llvm::ConstantFP*>(constant));
    } else if (llvm::ConstantInt::classof(constant)) {
        result = ConstantIntWrapper::of(static_cast<llvm::ConstantInt*>(constant));
    } else if (llvm::ConstantPointerNull::classof(constant)) {
        result = ConstantPointerNullWrapper::of(static_cast<llvm::ConstantPointerNull*>(constant));
    } else if (llvm::ConstantDataArray::classof(constant)) {
        result = ConstantDataArrayWrapper::of(static_cast<llvm::ConstantDataArray*>(constant));
    } else if (llvm::GlobalVariable::classof(constant)) {
        result = GlobalVariableWrapper::of(static_cast<llvm::GlobalVariable*>(constant));
    } else if (llvm::ConstantStruct::classof(constant)) {
        result = ConstantStructWrapper::of(static_cast<llvm::ConstantStruct*>(constant));
    } else if (llvm::ConstantArray::classof(constant)) {
        result = ConstantArrayWrapper::of(static_cast<llvm::ConstantArray*>(constant));
    } else if (llvm::UndefValue::classof(constant)) {
        result = UndefValueWrapper::of(static_cast<llvm::UndefValue*>(constant));
    } else {
        auto constructorFunction = Napi::GetFunction(Napi::New(env, constantTemplate()));
        Napi::Value argv[1] = { Napi::External::New(env, constant) };
        result = Napi::NewInstance(constructorFunction, 1, argv);
    }

    Napi::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(result);
}

Napi::Value ConstantWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Class Constructor Constant cannot be invoked without new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "Constant constructor requires: constant: External").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* constant = static_cast<llvm::Constant*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new ConstantWrapper { constant };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value ConstantWrapper::getNullValue(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "getNullValue needs to be called with: type: Type").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto* constant = llvm::Constant::getNullValue(type);

    return ConstantWrapper::of(constant);
}

Napi::Value ConstantWrapper::getAllOnesValue(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "getAllOnesValue needs to be called with: type: Type").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto* constant = llvm::Constant::getAllOnesValue(type);

    return ConstantWrapper::of(constant);
}

Napi::Value ConstantWrapper::isNullValue(const Napi::CallbackInfo& info) {
    auto* wrapper = ConstantWrapper::FromValue(info.Holder());
    return Napi::New(env, wrapper->getConstant()->isNullValue());
}

Napi::Value ConstantWrapper::isOneValue(const Napi::CallbackInfo& info) {
    auto* wrapper = ConstantWrapper::FromValue(info.Holder());
    return Napi::New(env, wrapper->getConstant()->isOneValue());
}

Napi::Value ConstantWrapper::isAllOnesValue(const Napi::CallbackInfo& info) {
    auto* wrapper = ConstantWrapper::FromValue(info.Holder());
    return Napi::New(env, wrapper->getConstant()->isAllOnesValue());
}

Napi::FunctionReference& ConstantWrapper::constantTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        Napi::FunctionReference localTemplate = Napi::Function::New(env, ConstantWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "Constant"));

        localTemplate->Inherit(Napi::New(env, ValueWrapper::valueTemplate()));

        Napi::SetMethod(localTemplate, "getNullValue", ConstantWrapper::getNullValue);
        Napi::SetMethod(localTemplate, "getAllOnesValue", ConstantWrapper::getAllOnesValue);
        Napi::SetPrototypeMethod(localTemplate, "isNullValue", ConstantWrapper::isNullValue);
        Napi::SetPrototypeMethod(localTemplate, "isOneValue", ConstantWrapper::isOneValue);
        Napi::SetPrototypeMethod(localTemplate, "isAllOnesValue", ConstantWrapper::isAllOnesValue);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

llvm::Constant *ConstantWrapper::getConstant() {
    return static_cast<llvm::Constant*>(getValue());
}