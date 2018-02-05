//
// Created by Micha Reiser on 13.03.17.
//

#include "call-inst.h"

NAN_MODULE_INIT(CallInstWrapper::Init) {
    auto callInstruction = Nan::GetFunction(Nan::New(callInstTemplate())).ToLocalChecked();

    Nan::Set(target, Nan::New("CallInst").ToLocalChecked(), callInstruction);
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

    auto callingConv = Nan::To<uint32_t>(value).FromJust();
    CallInstWrapper::FromValue(info.Holder())->getCallInst()->setCallingConv(callingConv);
}

NAN_METHOD(CallInstWrapper::addDereferenceableAttr) {
    if (info.Length() != 2 || !info[0]->IsUint32() || !info[1]->IsUint32()) {
        return Nan::ThrowTypeError("addDereferenceableAttr needs to be called with: argumentIndex: uint32, bytes: uint32");
    }

    auto* call = CallInstWrapper::FromValue(info.Holder())->getCallInst();
    auto index = Nan::To<uint32_t>(info[0]).FromJust();
    auto bytes = Nan::To<uint32_t>(info[1]).FromJust();

    call->addDereferenceableAttr(index, bytes);
}

NAN_METHOD(CallInstWrapper::hasRetAttr) {
    if (info.Length() != 1 || !info[0]->IsUint32()) {
        return Nan::ThrowTypeError("hasRetAttr needs to be called with: attrKind: AttrKind");
    }

    llvm::CallInst *call = CallInstWrapper::FromValue(info.Holder())->getCallInst();
    auto kind = static_cast<llvm::Attribute::AttrKind >(Nan::To<uint32_t>(info[0]).FromJust());
    bool hasRetAttr = false;

#if LLVM_VERSION_MAJOR == 4
    hasRetAttr = call->paramHasAttr(llvm::AttributeSet::ReturnIndex, kind);
#else
    hasRetAttr = call->hasRetAttr(kind);
#endif

    info.GetReturnValue().Set(hasRetAttr);
}

NAN_METHOD(CallInstWrapper::paramHasAttr) {
    if (info.Length() != 2 || !info[0]->IsUint32() || !info[1]->IsUint32()) {
        return Nan::ThrowTypeError("paramHasAttr needs to be called with: argNo: uint32, kind: AttrKind");
    }

    llvm::CallInst* call = CallInstWrapper::FromValue(info.Holder())->getCallInst();

    const auto index = Nan::To<uint32_t >(info[0]).FromJust();
    const auto kind = static_cast<llvm::Attribute::AttrKind >(Nan::To<uint32_t>(info[1]).FromJust());
    bool paramHasAttr = false;

#if LLVM_VERSION_MAJOR == 4
    paramHasAttr = call->paramHasAttr(index + 1, kind);
#else
    paramHasAttr = call->paramHasAttr(index, kind);
#endif
    info.GetReturnValue().Set(paramHasAttr);
}

NAN_METHOD(CallInstWrapper::getNumArgOperands) {
    auto* call = CallInstWrapper::FromValue(info.Holder())->getCallInst();
    info.GetReturnValue().Set(Nan::New(call->getNumArgOperands()));
}

Nan::Persistent<v8::FunctionTemplate>& CallInstWrapper::callInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(CallInstWrapper::New);
        localTemplate->SetClassName(Nan::New("CallInst").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(valueTemplate()));

        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("callingConv").ToLocalChecked(), CallInstWrapper::getCallingConv, CallInstWrapper::setCallingConv);
        Nan::SetPrototypeMethod(localTemplate, "addDereferenceableAttr", CallInstWrapper::addDereferenceableAttr);
        Nan::SetPrototypeMethod(localTemplate, "paramHasAttr", CallInstWrapper::paramHasAttr);
        Nan::SetPrototypeMethod(localTemplate, "hasRetAttr", CallInstWrapper::hasRetAttr);
        Nan::SetPrototypeMethod(localTemplate, "getNumArgOperands", CallInstWrapper::getNumArgOperands);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}