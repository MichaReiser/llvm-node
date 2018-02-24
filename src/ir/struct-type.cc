//
// Created by Micha Reiser on 13.03.17.
//

#include "struct-type.h"
#include "../util/string.h"

Napi::Object StructTypeWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto structType = Napi::GetFunction(Napi::New(env, structTypeTemplate()));
    (target).Set(Napi::String::New(env, "StructType"), structType);
}

Napi::Object StructTypeWrapper::of(llvm::StructType *type) {
    auto constructorFunction = Napi::GetFunction(Napi::New(env, structTypeTemplate()));
    Napi::Value args[1] = { Napi::External::New(env, type) };
    auto instance = Napi::NewInstance(constructorFunction, 1, args);

    Napi::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

bool StructTypeWrapper::isInstance(Napi::Value value) {
    Napi::Env env = value.Env();
    return Napi::New(env, structTypeTemplate())->HasInstance(value);
}

llvm::StructType* StructTypeWrapper::getStructType() {
    return static_cast<llvm::StructType*>(getType());
}

Napi::Value StructTypeWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "The StructType constructor needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "The StructType constructor needs to be called with: structType: External").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* structType = static_cast<llvm::StructType*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new StructTypeWrapper { structType };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value StructTypeWrapper::get(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !LLVMContextWrapper::isInstance(info[0]) || !info[1].IsArray() ||
            (info.Length() == 3 && !info[2].IsBoolean()) ||
            info.Length() > 3) {
        Napi::TypeError::New(env, "get needs to be called with: context: LLVMContext, elements: Type[], isPacked?: boolean").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    auto array = *info[1].As<Napi::Array>();
    std::vector<llvm::Type*> elements { array->Length() };

    for (size_t i = 0; i < array->Length(); ++i) {
        auto element = array->Get(i);

        if (!TypeWrapper::isInstance(element)) {
            Napi::TypeError::New(env, "expected Type").ThrowAsJavaScriptException();
            return env.Null();
        }

        elements[i] = TypeWrapper::FromValue(element)->getType();
    }

    bool isPacked = false;
    if (info.Length() == 3) {
        isPacked = info[2].As<Napi::Boolean>().Value();
    }

    auto* result = llvm::StructType::get(context, elements, isPacked);

    return StructTypeWrapper::of(result);
}

Napi::Value StructTypeWrapper::create(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0]) ||
            (info.Length() > 1 && !info[1].IsString()) ||
            info.Length() > 2) {
        Napi::TypeError::New(env, "create needs to be called with: context: LLVMContext, name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    std::string name {};
    if (info.Length() > 1) {
        name = ToString(info[1]);
    }

    auto* result = llvm::StructType::create(context, name);

    return StructTypeWrapper::of(result);
}

Napi::Value StructTypeWrapper::getName(const Napi::CallbackInfo& info) {
    auto* structType = StructTypeWrapper::FromValue(info.Holder())->getStructType();
    if (structType->isLiteral()) {
        return env.Undefined(); // Struct Types never have names;
    } else {
        std::string name = structType->getName().str();
        return Napi::New(env, name);
    }
}

void StructTypeWrapper::setName(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!value.IsString()) {
        Napi::TypeError::New(env, "name needs to be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* structType = StructTypeWrapper::FromValue(info.Holder())->getStructType();
    structType->setName(ToString(value));
}

Napi::Value StructTypeWrapper::getNumElements(const Napi::CallbackInfo& info) {
    auto* structType = StructTypeWrapper::FromValue(info.Holder())->getStructType();
    return Napi::New(env, structType->getNumElements());
}

Napi::Value StructTypeWrapper::getElementType(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !info[0].IsUint32()) {
        Napi::TypeError::New(env, "getElementType needs to be called with: index: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto index = info[0].As<Napi::Number>().Uint32Value();
    auto* type = StructTypeWrapper::FromValue(info.Holder())->getStructType()->getElementType(index);

    return TypeWrapper::of(type);
}

Napi::Value StructTypeWrapper::setBody(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !info[0].IsArray() ||
            (info.Length() > 1 && !info[1].IsBoolean()) ||
            info.Length() > 2) {
        Napi::TypeError::New(env, "setBody needs to be called with: elements: Type[], packed?: boolean").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* structType = StructTypeWrapper::FromValue(info.This())->getStructType();

    auto array = *info[0].As<Napi::Array>();
    std::vector<llvm::Type*> elements { array->Length() };

    for (size_t i = 0; i < array->Length(); ++i) {
        auto element = array->Get(i);

        if (!TypeWrapper::isInstance(element)) {
            Napi::TypeError::New(env, "expected Type").ThrowAsJavaScriptException();
            return env.Null();
        }

        elements[i] = TypeWrapper::FromValue(element)->getType();
    }

    bool packed = false;

    if (info.Length() > 1) {
        packed = info[1].As<Napi::Boolean>().Value();
    }

    structType->setBody(elements, packed);
}

Napi::FunctionReference& StructTypeWrapper::structTypeTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        Napi::FunctionReference localTemplate = Napi::Function::New(env, StructTypeWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "StructType"));

        localTemplate->Inherit(Napi::New(env, typeTemplate()));

        Napi::SetMethod(localTemplate, "create", StructTypeWrapper::create);
        Napi::SetMethod(localTemplate, "get", StructTypeWrapper::get);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "name"), StructTypeWrapper::getName, StructTypeWrapper::setName);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "numElements"), StructTypeWrapper::getNumElements);
        Napi::SetPrototypeMethod(localTemplate, "getElementType", StructTypeWrapper::getElementType);
        Napi::SetPrototypeMethod(localTemplate, "setBody", StructTypeWrapper::setBody);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}