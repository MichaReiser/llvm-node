//
// Created by Micha Reiser on 28.02.17.
//

#include "pointer-type.h"

NAN_MODULE_INIT(PointerTypeWrapper::Init) {
    auto pointerType = Nan::GetFunction(Nan::New(pointerTypeTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("PointerType").ToLocalChecked(), pointerType);
}

NAN_METHOD(PointerTypeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected pointer type pointer");
    }

    auto* type = static_cast<llvm::PointerType*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new PointerTypeWrapper { type };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(PointerTypeWrapper::get) {
    if (info.Length() != 2 || !TypeWrapper::isInstance(info[0]) || !info[1]->IsUint32()) {
        return Nan::ThrowTypeError("PointerTypeWrapper.get needs to be called with: elementType: Type, addressSpace: uint32");
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    uint32_t as = Nan::To<uint32_t>(info[1]).FromJust();

    auto* pointerType = llvm::PointerType::get(type, as);

    info.GetReturnValue().Set(PointerTypeWrapper::of(pointerType));
}

NAN_GETTER(PointerTypeWrapper::getElementType) {
    auto* pointerType = PointerTypeWrapper::FromValue(info.Holder())->getPointerType();
    auto* elementType = pointerType->getElementType();

    info.GetReturnValue().Set(TypeWrapper::of(elementType));
}

v8::Local<v8::Object> PointerTypeWrapper::of(llvm::PointerType *type) {
    Nan::EscapableHandleScope escapeScope {};

    v8::Local<v8::FunctionTemplate> functionTemplate = Nan::New(pointerTypeTemplate());
    auto constructorFunction = Nan::GetFunction(functionTemplate).ToLocalChecked();
    v8::Local<v8::Value> argv[1] = { Nan::New<v8::External>(type) };
    v8::Local<v8::Object> object = Nan::NewInstance(constructorFunction, 1, argv).ToLocalChecked();

    return escapeScope.Escape(object);
}

Nan::Persistent<v8::FunctionTemplate>& PointerTypeWrapper::pointerTypeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> persistentTemplate {};

    if (persistentTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> pointerTypeTemplate = Nan::New<v8::FunctionTemplate>(PointerTypeWrapper::New);

        Nan::SetMethod(pointerTypeTemplate, "get", PointerTypeWrapper::get);
        pointerTypeTemplate->SetClassName(Nan::New("PointerType").ToLocalChecked());
        pointerTypeTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        pointerTypeTemplate->Inherit(Nan::New(typeTemplate()));

        Nan::SetAccessor(pointerTypeTemplate->InstanceTemplate(), Nan::New("elementType").ToLocalChecked(), PointerTypeWrapper::getElementType);

        persistentTemplate.Reset(pointerTypeTemplate);
    }

    return persistentTemplate;
}

bool PointerTypeWrapper::isInstance(v8::Local<v8::Value> object) {
    return Nan::New(pointerTypeTemplate())->HasInstance(object);
}

llvm::PointerType *PointerTypeWrapper::getPointerType() {
    return static_cast<llvm::PointerType*>(getType());
}

