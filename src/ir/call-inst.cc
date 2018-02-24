//
// Created by Micha Reiser on 13.03.17.
//

#include "call-inst.h"

Napi::Object CallInstWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto callInstruction = Napi::GetFunction(Napi::New(env, callInstTemplate()));

    (target).Set(Napi::String::New(env, "CallInst"), callInstruction);
}

Napi::Object CallInstWrapper::of(llvm::CallInst* inst) {
    Napi::EscapableHandleScope escapeScope {};
    auto constructorFunction = Napi::GetFunction(Napi::New(env, callInstTemplate()));
    Napi::Value args[1] = { Napi::External::New(env, inst) };
    auto instance = Napi::NewInstance(constructorFunction, 1, args);

    return escapeScope.Escape(instance);
}

bool CallInstWrapper::isInstance(Napi::Value value) {
    Napi::Env env = value.Env();
    return Napi::New(env, callInstTemplate())->HasInstance(value);
}

llvm::CallInst* CallInstWrapper::getCallInst() {
    return static_cast<llvm::CallInst*>(getValue());
}

Napi::Value CallInstWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "CallInst Constructor needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "CallInst Constructor needs to be called with: callInst: External").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* callInst = static_cast<llvm::CallInst*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new CallInstWrapper { callInst };
    wrapper->Wrap(info.This());
    return info.This();
}

Napi::Value CallInstWrapper::getCallingConv(const Napi::CallbackInfo& info) {
    auto* callInst = CallInstWrapper::FromValue(info.Holder())->getCallInst();
    auto callingConv = callInst->getCallingConv();
    return Napi::New(env, callingConv);
}

void CallInstWrapper::setCallingConv(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if (!value->IsUint32()) {
        Napi::TypeError::New(env, "The Calling Convention needs to be a value from llvm.CallingConv").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto callingConv = value.As<Napi::Number>().Uint32Value();
    CallInstWrapper::FromValue(info.Holder())->getCallInst()->setCallingConv(callingConv);
}

Napi::Value CallInstWrapper::addDereferenceableAttr(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !info[0].IsUint32() || !info[1].IsUint32()) {
        Napi::TypeError::New(env, "addDereferenceableAttr needs to be called with: argumentIndex: uint32, bytes: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* call = CallInstWrapper::FromValue(info.Holder())->getCallInst();
    auto index = info[0].As<Napi::Number>().Uint32Value();
    auto bytes = info[1].As<Napi::Number>().Uint32Value();

    call->addDereferenceableAttr(index, bytes);
}

Napi::Value CallInstWrapper::hasRetAttr(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !info[0].IsUint32()) {
        Napi::TypeError::New(env, "hasRetAttr needs to be called with: attrKind: AttrKind").ThrowAsJavaScriptException();
        return env.Null();
    }

    llvm::CallInst *call = CallInstWrapper::FromValue(info.Holder())->getCallInst();
    auto kind = static_cast<llvm::Attribute::AttrKind >(info[0].As<Napi::Number>().Uint32Value());
    bool hasRetAttr = false;

#if LLVM_VERSION_MAJOR == 4
    hasRetAttr = call->paramHasAttr(llvm::AttributeSet::ReturnIndex, kind);
#else
    hasRetAttr = call->hasRetAttr(kind);
#endif

    return hasRetAttr;
}

Napi::Value CallInstWrapper::paramHasAttr(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !info[0].IsUint32() || !info[1].IsUint32()) {
        Napi::TypeError::New(env, "paramHasAttr needs to be called with: argNo: uint32, kind: AttrKind").ThrowAsJavaScriptException();
        return env.Null();
    }

    llvm::CallInst* call = CallInstWrapper::FromValue(info.Holder())->getCallInst();

    const auto index = Napi::To<uint32_t >(info[0]);
    const auto kind = static_cast<llvm::Attribute::AttrKind >(info[1].As<Napi::Number>().Uint32Value());
    bool paramHasAttr = false;

#if LLVM_VERSION_MAJOR == 4
    paramHasAttr = call->paramHasAttr(index + 1, kind);
#else
    paramHasAttr = call->paramHasAttr(index, kind);
#endif
    return paramHasAttr;
}

Napi::Value CallInstWrapper::getNumArgOperands(const Napi::CallbackInfo& info) {
    auto* call = CallInstWrapper::FromValue(info.Holder())->getCallInst();
    return Napi::New(env, call->getNumArgOperands());
}

Napi::FunctionReference& CallInstWrapper::callInstTemplate() {
    static Napi::FunctionReference functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Napi::Function::New(env, CallInstWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "CallInst"));

        localTemplate->Inherit(Napi::New(env, valueTemplate()));

        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "callingConv"), CallInstWrapper::getCallingConv, CallInstWrapper::setCallingConv);
        Napi::SetPrototypeMethod(localTemplate, "addDereferenceableAttr", CallInstWrapper::addDereferenceableAttr);
        Napi::SetPrototypeMethod(localTemplate, "paramHasAttr", CallInstWrapper::paramHasAttr);
        Napi::SetPrototypeMethod(localTemplate, "hasRetAttr", CallInstWrapper::hasRetAttr);
        Napi::SetPrototypeMethod(localTemplate, "getNumArgOperands", CallInstWrapper::getNumArgOperands);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}