//
// Created by Micha Reiser on 13.03.17.
//

#include "call-inst.h"

NAN_MODULE_INIT(CallInstWrapper::Init) {
    auto object = Nan::New<v8::Object>();

    Nan::Set(target, Nan::New("CallInst").ToLocalChecked(), object);
}

v8::Local<v8::Object> CallInstWrapper::of(llvm::CallInst* inst) {
    Nan::EscapableHandleScope escapeScope {};
    auto constructorFunction = Nan::GetFunction(Nan::New(callInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(inst) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    return escapeScope.Escape(instance);
}

bool CallInstWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(callInstTemplate())->HasInstance(value);
}

llvm::CallInst* CallInstWrapper::getCallInst() {
    return static_cast<llvm::CallInst*>(getValue());
}

NAN_METHOD(CallInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("CallInst Constructor needs to be called with new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("CallInst Constructor needs to be called with: callInst: External");
    }

    auto* callInst = static_cast<llvm::CallInst*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new CallInstWrapper { callInst };
    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_GETTER(CallInstWrapper::getCallingConv) {
    auto* callInst = CallInstWrapper::FromValue(info.Holder())->getCallInst();
    auto callingConv = callInst->getCallingConv();
    info.GetReturnValue().Set(Nan::New(callingConv));
}

NAN_SETTER(CallInstWrapper::setCallingConv) {
    if (!value->IsUint32()) {
        return Nan::ThrowTypeError("The Calling Convention needs to be a value from llvm.CallingConv");
    }

    auto callingConv = Nan::To<uint32_t>(value).ToChecked();
    CallInstWrapper::FromValue(info.Holder())->getCallInst()->setCallingConv(callingConv);
}

Nan::Persistent<v8::FunctionTemplate>& CallInstWrapper::callInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(CallInstWrapper::New);
        localTemplate->SetClassName(Nan::New("CallInst").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(valueTemplate()));

        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("callingConv").ToLocalChecked(), CallInstWrapper::getCallingConv, CallInstWrapper::setCallingConv);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}