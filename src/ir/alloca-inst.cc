//
// Created by Micha Reiser on 03.03.17.
//

#include "alloca-inst.h"
#include "type.h"

NAN_MODULE_INIT(AllocaInstWrapper::Init) {
    // nothing to do?
}

v8::Local<v8::Object> AllocaInstWrapper::of(llvm::AllocaInst* inst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(allocaInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(inst) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

llvm::AllocaInst* AllocaInstWrapper::getAllocaInst() {
    return static_cast<llvm::AllocaInst*>(getValue());
}

NAN_METHOD(AllocaInstWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor AllocaInst cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("AllocaInst constructor needs to be called with: inst: External");
    }

    auto* inst = static_cast<llvm::AllocaInst*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new AllocaInstWrapper { inst };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_GETTER(AllocaInstWrapper::getAllocatedType) {
    auto* wrapper = AllocaInstWrapper::FromValue(info.Holder());
    auto* type = wrapper->getAllocaInst()->getAllocatedType();

    info.GetReturnValue().Set(TypeWrapper::of(type));
}

NAN_SETTER(AllocaInstWrapper::setAllocatedType) {
    if (!TypeWrapper::isInstance(value)) {
        return Nan::ThrowTypeError("allocatedType needs to be a Type");
    }

    auto* type = TypeWrapper::FromValue(value)->getType();
    auto* wrapper = AllocaInstWrapper::FromValue(info.Holder());
    wrapper->getAllocaInst()->setAllocatedType(type);
}

Nan::Persistent<v8::FunctionTemplate>& AllocaInstWrapper::allocaInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(AllocaInstWrapper::New);
        localTemplate->SetClassName(Nan::New("AllocaInst").ToLocalChecked());
        localTemplate->Inherit(Nan::New(valueTemplate()));
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("allocatedType").ToLocalChecked(), AllocaInstWrapper::getAllocatedType, AllocaInstWrapper::setAllocatedType);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}