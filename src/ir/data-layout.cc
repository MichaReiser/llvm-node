//
// Created by Micha Reiser on 28.02.17.
//

#include "../util/string.h"
#include "data-layout.h"
#include "type.h"

Napi::FunctionReference DataLayoutWrapper::functionTemplate {};

Napi::Object DataLayoutWrapper::of(llvm::DataLayout layout) {
    Napi::FunctionReference localNapi::FunctionReference = Napi::New(env, functionTemplate);
    Napi::Object object = Napi::NewInstance(localNapi::FunctionReference->InstanceTemplate());

    DataLayoutWrapper* wrapper = new DataLayoutWrapper(layout);
    wrapper->Wrap(object);

    Napi::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(object);
}

Napi::Object DataLayoutWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::FunctionReference tpl = Napi::Function::New(env, New);

    tpl->SetClassName(Napi::String::New(env, "DataLayout"));

    Napi::SetPrototypeMethod(tpl, "getStringRepresentation", DataLayoutWrapper::getStringRepresentation);
    Napi::SetPrototypeMethod(tpl, "getPrefTypeAlignment", DataLayoutWrapper::getPrefTypeAlignment);
    Napi::SetPrototypeMethod(tpl, "getTypeStoreSize", DataLayoutWrapper::getTypeStoreSize);
    Napi::SetPrototypeMethod(tpl, "getPointerSize", DataLayoutWrapper::getPointerSize);
    Napi::SetPrototypeMethod(tpl, "getIntPtrType", DataLayoutWrapper::getIntPtrType);

    functionTemplate.Reset(tpl);

    (target).Set(Napi::String::New(env, "DataLayout"), Napi::GetFunction(tpl));
}

Napi::Value DataLayoutWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "DataLayout functionTemplate needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() < 1 && !info[0].IsString()) {
        Napi::TypeError::New(env, "DataLayout functionTemplate needs to be called with single string argument").ThrowAsJavaScriptException();
        return env.Null();
    }

    llvm::DataLayout layout { ToString(info[0].ToString()) };
    DataLayoutWrapper* wrapper = new DataLayoutWrapper { layout };
    wrapper->Wrap(info.This());
    return info.This();
}

Napi::Value DataLayoutWrapper::getStringRepresentation(const Napi::CallbackInfo& info) {
    DataLayoutWrapper* wrapper = DataLayoutWrapper::FromValue(info.Holder());
    std::string representation = wrapper->layout.getStringRepresentation();

    return v8::String::NewFromUtf8(info.GetIsolate(), representation.c_str());
}

Napi::Value DataLayoutWrapper::getPointerSize(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !info[0].IsUint32()) {
        Napi::TypeError::New(env, "getPointerSize needs to be called with: AS: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto as = info[0].As<Napi::Number>().Uint32Value();
    auto dataLayout = DataLayoutWrapper::FromValue(info.Holder())->getDataLayout();
    return Napi::New(env, dataLayout.getPointerSize(as));
}

Napi::Value DataLayoutWrapper::getPrefTypeAlignment(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "getPrefTypeAlignment needs to be called with: type: Type").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto dataLayout = DataLayoutWrapper::FromValue(info.Holder())->getDataLayout();

    return Napi::New(env, dataLayout.getPrefTypeAlignment(type));
}

Napi::Value DataLayoutWrapper::getTypeStoreSize(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "getTypeStoreSize needs to be called with: type: Type").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto dataLayout = DataLayoutWrapper::FromValue(info.Holder())->getDataLayout();

    auto size = dataLayout.getTypeStoreSize(type);
     assert (size < UINT32_MAX && "V8 does not support uint64 but size overflows uint32"); // v8 does not support uint64_t :(

    return Napi::New(env, static_cast<uint32_t>(size));
}

Napi::Value DataLayoutWrapper::getIntPtrType(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0]) ||
            (info.Length() == 2 && !info[1].IsUint32()) ||
            info.Length() > 2) {
        Napi::TypeError::New(env, "getIntPtrType needs to be called with: context: LLVMContext, addressSpace?: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    const auto dataLayout = DataLayoutWrapper::FromValue(info.Holder())->getDataLayout();
    uint32_t addressSpace {};

    if (info.Length() == 2) {
        addressSpace = info[1].As<Napi::Number>().Uint32Value();
    }

    auto* type = dataLayout.getIntPtrType(context, addressSpace);
    return TypeWrapper::of(type);
}

llvm::DataLayout DataLayoutWrapper::getDataLayout() {
    return layout;
}

bool DataLayoutWrapper::isInstance(Napi::Value value) {
    Napi::Env env = value.Env();
    return Napi::New(env, functionTemplate)->HasInstance(value);
}
