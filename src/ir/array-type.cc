//
// Created by Micha Reiser on 11.03.17.
//

#include "array-type.h"

NAN_MODULE_INIT(ArrayTypeWrapper::Init) {
    auto arrayType = Nan::GetFunction(Nan::New(arrayTypeTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("ArrayType").ToLocalChecked(), arrayType);
}

v8::Local<v8::Object> ArrayTypeWrapper::of(llvm::ArrayType* arrayType) {
    auto constructorFunction = Nan::GetFunction(Nan::New(arrayTypeTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External> (arrayType) };

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

bool ArrayTypeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(arrayTypeTemplate())->HasInstance(value);
}

llvm::ArrayType* ArrayTypeWrapper::getArrayType() {
    return static_cast<llvm::ArrayType*>(getType());
}

NAN_METHOD(ArrayTypeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("ArrayType constructor needs to be called with: arrayType: external");
    }

    auto* arrayType = static_cast<llvm::ArrayType*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ArrayTypeWrapper { arrayType };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ArrayTypeWrapper::get) {
    if (info.Length() != 2 || !TypeWrapper::isInstance(info[0]) || !info[1]->IsUint32()) {
        return Nan::ThrowTypeError("get needs to be called with: elementType: Type, numElements: uint32");
    }

    auto* elementType = TypeWrapper::FromValue(info[0])->getType();
    auto numElements = Nan::To<uint32_t>(info[1]).FromJust();

    auto* arrayType = llvm::ArrayType::get(elementType, numElements);

    info.GetReturnValue().Set(ArrayTypeWrapper::of(arrayType));
}

NAN_GETTER(ArrayTypeWrapper::getNumElements) {
    auto* arrayType = ArrayTypeWrapper::FromValue(info.Holder())->getArrayType();
    uint32_t numElements = arrayType->getNumElements();

    info.GetReturnValue().Set(Nan::New(numElements));
}

NAN_GETTER(ArrayTypeWrapper::getElementType) {
    auto* arrayType = ArrayTypeWrapper::FromValue(info.Holder())->getArrayType();
    auto* elementType = arrayType->getElementType();

    info.GetReturnValue().Set(TypeWrapper::of(elementType));
}

v8::Persistent<v8::FunctionTemplate>& ArrayTypeWrapper::arrayTypeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        auto arrayTypeWrapperTemplate = Nan::New<v8::FunctionTemplate>(ArrayTypeWrapper::New);
        arrayTypeWrapperTemplate->SetClassName(Nan::New("ArrayType").ToLocalChecked());
        arrayTypeWrapperTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        arrayTypeWrapperTemplate->Inherit(Nan::New(typeTemplate()));

        Nan::SetMethod(arrayTypeWrapperTemplate, "get", ArrayTypeWrapper::get);
        Nan::SetAccessor(arrayTypeWrapperTemplate->InstanceTemplate(), Nan::New("numElements").ToLocalChecked(), ArrayTypeWrapper::getNumElements);
        Nan::SetAccessor(arrayTypeWrapperTemplate->InstanceTemplate(), Nan::New("elementType").ToLocalChecked(), ArrayTypeWrapper::getElementType);

        tmpl.Reset(arrayTypeWrapperTemplate);
    }

    return tmpl;
}
