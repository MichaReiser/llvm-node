//
// Created by Micha Reiser on 03.03.17.
//

#include "alloca-inst.h"
#include "type.h"

NAN_MODULE_INIT(AllocaInstWrapper::Init) {
    auto allocaInst = Nan::GetFunction(Nan::New(allocaInstTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("AllocaInst").ToLocalChecked(), allocaInst);
}

v8::Local<v8::Object> AllocaInstWrapper::of(llvm::AllocaInst* inst) {
    auto constructorFunction = Nan::GetFunction(Nan::New(allocaInstTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(inst) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
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

NAN_GETTER(AllocaInstWrapper::getAlignment) {
    auto* wrapper = AllocaInstWrapper::FromValue(info.Holder());

    info.GetReturnValue().Set(Nan::New(wrapper->getAllocaInst()->getAlignment()));
}

NAN_SETTER(AllocaInstWrapper::setAlignment) {
    if (!value->IsUint32()) {
        return Nan::ThrowTypeError("alignment needs to be an uint32");
    }

    auto* wrapper = AllocaInstWrapper::FromValue(info.Holder());
    wrapper->getAllocaInst()->setAlignment(Nan::To<uint32_t>(value).FromJust());
}

NAN_GETTER(AllocaInstWrapper::getArraySize) {
    auto* wrapper = AllocaInstWrapper::FromValue(info.Holder());

    llvm::Value* arraySize = wrapper->getAllocaInst()->getArraySize();
    if (arraySize == nullptr) {
        info.GetReturnValue().Set(Nan::Null());
    } else {
        info.GetReturnValue().Set(ValueWrapper::of(arraySize));
    }
}

Nan::Persistent<v8::FunctionTemplate>& AllocaInstWrapper::allocaInstTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(AllocaInstWrapper::New);
        localTemplate->SetClassName(Nan::New("AllocaInst").ToLocalChecked());
        localTemplate->Inherit(Nan::New(valueTemplate()));
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("allocatedType").ToLocalChecked(), AllocaInstWrapper::getAllocatedType, AllocaInstWrapper::setAllocatedType);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("alignment").ToLocalChecked(), AllocaInstWrapper::getAlignment, AllocaInstWrapper::setAlignment);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("arraySize").ToLocalChecked(), AllocaInstWrapper::getArraySize);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}
