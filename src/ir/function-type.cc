//
// Created by Micha Reiser on 01.03.17.
//

#include "function-type.h"
#include "../util/string.h"

Napi::Object FunctionTypeWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto functionType = Napi::GetFunction(Napi::New(env, functionTypeTemplate()));
    (target).Set(Napi::String::New(env, "FunctionType"), functionType);
}

Napi::Value FunctionTypeWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Constructor needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() < 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "Expected type pointer").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* functionType = static_cast<llvm::FunctionType*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new FunctionTypeWrapper { functionType };
    wrapper->Wrap(info.This());

    return info.This();
}


llvm::FunctionType *FunctionTypeWrapper::getFunctionType() {
    return static_cast<llvm::FunctionType*>(this->type);
}

Napi::Object FunctionTypeWrapper::Create(llvm::FunctionType *functionType) {
    Napi::EscapableHandleScope escapeScope {};

    Napi::FunctionReference functionTemplate = Napi::New(env, functionTypeTemplate());
    auto constructorFunction = Napi::GetFunction(functionTemplate);
    Napi::Value argv[1] = { Napi::External::New(env, functionType) };
    Napi::Object object = Napi::NewInstance(constructorFunction, 1, argv);

    return escapeScope.Escape(object);
}


Napi::Value FunctionTypeWrapper::get(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !TypeWrapper::isInstance(info[0])
        || (!(info.Length() == 2 && info[1].IsBoolean())
            && !(info.Length() == 3 && info[1].IsArray() && info[2].IsBoolean()))) {
        Napi::TypeError::New(env, "Get needs to be called with: result: Type, params: Type[], isVarArg: boolean").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto returnType = TypeWrapper::FromValue(info[0])->getType();
    llvm::FunctionType* functionType {};

    if (info.Length() == 2) {
        auto varArg = info[1].As<Napi::Boolean>().Value();

        functionType = llvm::FunctionType::get(returnType, varArg);
    } else {
        auto argumentTypesArray = *info[1].As<Napi::Array>();
        std::vector<llvm::Type *> argumentTypes {argumentTypesArray->Length()};

        for (size_t i = 0; i < argumentTypesArray->Length(); ++i) {
            auto argumentTypeObject = argumentTypesArray->Get(i);
            if (!TypeWrapper::isInstance(argumentTypeObject)) {
                Napi::TypeError::New(env, "Expected Type in arguments array").ThrowAsJavaScriptException();
                return env.Null();
            }

            argumentTypes[i] = TypeWrapper::FromValue(argumentTypeObject)->getType();
        }

        auto varArg = info[2].As<Napi::Boolean>().Value();

        functionType = llvm::FunctionType::get(returnType, argumentTypes, varArg);
    }

    return FunctionTypeWrapper::Create(functionType);
}

Napi::Value FunctionTypeWrapper::isValidReturnType(const Napi::CallbackInfo& info) {
    if (info.Length() != 1) {
        Napi::TypeError::New(env, "isValidReturnType takes exactly one argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!TypeWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "a Type is required").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto returnTypeWrapper = TypeWrapper::FromValue(info[0]);
    return Napi::New(env, llvm::FunctionType::isValidArgumentType(returnTypeWrapper->getType()));
}

Napi::Value FunctionTypeWrapper::isValidArgumentType(const Napi::CallbackInfo& info) {
    if (info.Length() != 1) {
        Napi::TypeError::New(env, "isValidArgumentType takes exactly one argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!TypeWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "a Type is required").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto argumentTypeWrapper = TypeWrapper::FromValue(info[0]);
    return Napi::New(env, llvm::FunctionType::isValidArgumentType(argumentTypeWrapper->getType()));
}

Napi::Value FunctionTypeWrapper::getReturnType(const Napi::CallbackInfo& info) {
    auto* functionTypeWrapper = FunctionTypeWrapper::FromValue(info.Holder());
    auto* returnType = functionTypeWrapper->getFunctionType()->getReturnType();
    return TypeWrapper::of(returnType);
}

Napi::Value FunctionTypeWrapper::isVarArg(const Napi::CallbackInfo& info) {
    auto* functionTypeWrapper = FunctionTypeWrapper::FromValue(info.Holder());
    return Napi::New(env, functionTypeWrapper->getFunctionType()->isVarArg());
}

Napi::Value FunctionTypeWrapper::getNumParams(const Napi::CallbackInfo& info) {
    auto* functionTypeWrapper = FunctionTypeWrapper::FromValue(info.Holder());
    return Napi::New(env, functionTypeWrapper->getFunctionType()->getNumParams());
}

Napi::Value FunctionTypeWrapper::getParams(const Napi::CallbackInfo& info) {
    auto* functionTypeWrapper = FunctionTypeWrapper::FromValue(info.Holder());
    auto result = Napi::Array::New(env, functionTypeWrapper->getFunctionType()->getNumParams());

    uint32_t i = 0;
    for (auto paramsIterator = functionTypeWrapper->getFunctionType()->param_begin(); paramsIterator !=
            functionTypeWrapper->getFunctionType()->param_end(); ++paramsIterator) {
        result.Set(i, TypeWrapper::of(*paramsIterator));
        ++i;
    }

    return result;
}

Napi::Value FunctionTypeWrapper::getParamType(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 && !info[0].IsUint32()) {
        Napi::TypeError::New(env, "A positive int is required").ThrowAsJavaScriptException();
        return env.Null();
    }
    auto* functionTypeWrapper = FunctionTypeWrapper::FromValue(info.Holder());

    unsigned index = Napi::To<unsigned>(info[0]);
    if (index < functionTypeWrapper->getFunctionType()->getNumParams()) {
        auto* paramType = functionTypeWrapper->getFunctionType()->getParamType(index);

        return TypeWrapper::of(paramType);
    } else {
        Napi::RangeError::New(env, "Index out of range").ThrowAsJavaScriptException();
        return env.Null();
    }
}

Napi::FunctionReference& FunctionTypeWrapper::functionTypeTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        Napi::FunctionReference localTemplate = Napi::Function::New(env, FunctionTypeWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "FunctionType"));


        Napi::SetMethod(localTemplate, "get", FunctionTypeWrapper::get);
        Napi::SetMethod(localTemplate, "isValidReturnType", FunctionTypeWrapper::isValidReturnType);
        Napi::SetMethod(localTemplate, "isValidArgumentType", FunctionTypeWrapper::isValidArgumentType);

        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "returnType"), FunctionTypeWrapper::getReturnType);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "isVarArg"), FunctionTypeWrapper::isVarArg);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "numParams"), FunctionTypeWrapper::getNumParams);
        Napi::SetPrototypeMethod(localTemplate, "getParams", FunctionTypeWrapper::getParams);
        Napi::SetPrototypeMethod(localTemplate, "getParamType", FunctionTypeWrapper::getParamType);

        Napi::FunctionReference localTypeTemplate = Napi::New(env, TypeWrapper::typeTemplate());
        localTemplate->Inherit(localTypeTemplate);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

bool FunctionTypeWrapper::isInstance(Napi::Value value) {
    Napi::Env env = value.Env();
    return Napi::New(env, functionTypeTemplate())->HasInstance(value);
}

