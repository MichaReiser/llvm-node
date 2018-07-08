//
// Created by Micha Reiser on 28.02.17.
//

#include "pointer-type.h"

Napi::FunctionReference PointerTypeWrapper::constructor;

void PointerTypeWrapper::Init(Napi::Env env, Napi::Object &exports){
    Napi::HandleScope scope { env };

    Napi::Function func = DefineClass(env, "PointerType", {
        StaticMethod("get", PointerTypeWrapper::get),
        InstanceAccessor("elementType", &PointerTypeWrapper::getElementType, nullptr)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("PointerType", func);
}

Napi::Object PointerTypeWrapper::of(Napi::Env env, llvm::PointerType *type) {
    return constructor.New({ Napi::External<llvm::PointerType>::New(env, type )});
}

bool PointerTypeWrapper::isInstanceOfType(const Napi::Value& value) {
    return value.As<Napi::Object>().InstanceOf(constructor.Value());
}

PointerTypeWrapper::PointerTypeWrapper(const Napi::CallbackInfo &info): Napi::ObjectWrap<PointerTypeWrapper> { info } {
    auto env = info.Env();

    if (!info.IsConstructCall()) {
        throw Napi::TypeError::New(env, "Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0].IsExternal()) {
        throw Napi::TypeError::New(env, "Expected pointer type pointer");
    }

    auto external = info[0].As<Napi::External<llvm::PointerType>>();
    pointerType = external.Data();
}

Napi::Value PointerTypeWrapper::get(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    if (info.Length() != 2 || !TypeWrapper::isInstanceOfType(info[0]) || !info[1].IsNumber()) {
        throw Napi::TypeError::New(env, "PointerTypeWrapper.get needs to be called with: elementType: Type, addressSpace: uint32");
    }

    auto* type = TypeWrapper::Unwrap(info[0].As<Napi::Object>())->getType();
    uint32_t as = info[1].As<Napi::Number>();

    auto* pointerType = llvm::PointerType::get(type, as);

    return PointerTypeWrapper::of(env, pointerType);
}

Napi::Value PointerTypeWrapper::getElementType(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    auto* elementType = pointerType->getElementType();
    return TypeWrapper::of(env, elementType);
}

llvm::PointerType *PointerTypeWrapper::getPointerType() {
    return pointerType;
}

