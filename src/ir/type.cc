//
// Created by Micha Reiser on 28.02.17.
//

#include "type.h"
#include "function-type.h"

NAN_MODULE_INIT(TypeWrapper::Init) {
    auto typeObj = Nan::New<v8::Object>(getObjectWithStaticMethods());

    auto typeIds = Nan::New<v8::Object>();
    Nan::Set(typeIds, Nan::New("VoidTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::VoidTyID));
    Nan::Set(typeIds, Nan::New("HalfTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::HalfTyID));
    Nan::Set(typeIds, Nan::New("FloatTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::FloatTyID));
    Nan::Set(typeIds, Nan::New("DoubleTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::DoubleTyID));
    Nan::Set(typeIds, Nan::New("X86_FP80TyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::X86_FP80TyID));
    Nan::Set(typeIds, Nan::New("FP128TyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::FP128TyID));
    Nan::Set(typeIds, Nan::New("PPC_FP128TyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::PPC_FP128TyID));
    Nan::Set(typeIds, Nan::New("LabelTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::LabelTyID));
    Nan::Set(typeIds, Nan::New("MetadataTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::MetadataTyID));
    Nan::Set(typeIds, Nan::New("X86_MMXTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::X86_MMXTyID));
    Nan::Set(typeIds, Nan::New("TokenTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::TokenTyID));
    Nan::Set(typeIds, Nan::New("IntegerTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::IntegerTyID));
    Nan::Set(typeIds, Nan::New("FunctionTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::FunctionTyID));
    Nan::Set(typeIds, Nan::New("StructTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::StructTyID));
    Nan::Set(typeIds, Nan::New("ArrayTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::ArrayTyID));
    Nan::Set(typeIds, Nan::New("PointerTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::PointerTyID));
    Nan::Set(typeIds, Nan::New("VectorTyID").ToLocalChecked(), Nan::New(llvm::Type::TypeID::VectorTyID));

    Nan::Set(typeObj, Nan::New("TypeID").ToLocalChecked(), typeIds);

    Nan::Set(target, Nan::New("Type").ToLocalChecked(), typeObj);


}

NAN_METHOD(TypeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto* type = static_cast<llvm::Type*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new TypeWrapper { type };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

typedef llvm::Type* (getTypeFn)(llvm::LLVMContext&);
template<getTypeFn method>
NAN_METHOD(TypeWrapper::getType) {
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("getType needs to be called with the context");
    }

    auto context = LLVMContextWrapper::FromValue(info[0]);

    auto* type = method(context->getContext());
    auto wrapped = TypeWrapper::of(type);
    info.GetReturnValue().Set(wrapped);
}

typedef llvm::IntegerType* (getIntTypeFn)(llvm::LLVMContext&);
template<getIntTypeFn method>
NAN_METHOD(TypeWrapper::getIntType) {
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("getIntTy needs to be called with the context");
    }

    auto context = LLVMContextWrapper::FromValue(info[0]);

    auto* type = method(context->getContext());
    auto wrapped = TypeWrapper::of(type);
    info.GetReturnValue().Set(wrapped);
}

v8::Local<v8::Object> TypeWrapper::of(llvm::Type *type) {
    if (type->isFunctionTy()) {
        return FunctionTypeWrapper::Create(static_cast<llvm::FunctionType*>(type));
    }

    Nan::EscapableHandleScope escapeScope {};

    v8::Local<v8::FunctionTemplate> functionTemplate = Nan::New(typeTemplate());
    auto constructorFunction = Nan::GetFunction(functionTemplate).ToLocalChecked();
    v8::Local<v8::Value> argv[1] = { Nan::New<v8::External>(type) };
    v8::Local<v8::Object> object = Nan::NewInstance(constructorFunction, 1, argv).ToLocalChecked();

    return escapeScope.Escape(object);
}

typedef bool (llvm::Type::*isTy)() const;
template<isTy method>
NAN_METHOD(TypeWrapper::isOfType) {
    auto* type = TypeWrapper::FromValue(info.Holder())->getType();
    auto result = Nan::New((type->*method)());
    info.GetReturnValue().Set(result);
}

NAN_GETTER(TypeWrapper::getTypeID) {
    auto* wrapper = TypeWrapper::FromValue(info.Holder());
    auto result = Nan::New(wrapper->type->getTypeID());
    info.GetReturnValue().Set(result);
}

Nan::Persistent<v8::FunctionTemplate>& TypeWrapper::typeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> persistentTemplate {};

    if (persistentTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> typeTemplate = Nan::New<v8::FunctionTemplate>(TypeWrapper::New);

        typeTemplate->SetClassName(Nan::New("Type").ToLocalChecked());
        typeTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetPrototypeMethod(typeTemplate, "isVoidTy", &TypeWrapper::isOfType<&llvm::Type::isVoidTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isFloatTy", &TypeWrapper::isOfType<&llvm::Type::isFloatTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isDoubleTy", &TypeWrapper::isOfType<&llvm::Type::isDoubleTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isLabelTy", &TypeWrapper::isOfType<&llvm::Type::isLabelTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isIntegerTy", &TypeWrapper::isOfType<&llvm::Type::isIntegerTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isFunctionTy", &TypeWrapper::isOfType<&llvm::Type::isFunctionTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isStructTy", &TypeWrapper::isOfType<&llvm::Type::isStructTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isArrayTy", &TypeWrapper::isOfType<&llvm::Type::isArrayTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isPointerTy", &TypeWrapper::isOfType<&llvm::Type::isPointerTy>);
        Nan::SetAccessor(typeTemplate->InstanceTemplate(), Nan::New("typeID").ToLocalChecked(), TypeWrapper::getTypeID);

        persistentTemplate.Reset(typeTemplate);
    }

    return persistentTemplate;
}

Nan::Persistent<v8::Object>& TypeWrapper::getObjectWithStaticMethods() {
    static Nan::Persistent<v8::Object> object {};

    if (object.IsEmpty()) {
        v8::Local<v8::Object> localObject = Nan::New<v8::Object>();
        Nan::SetMethod(localObject, "getDoubleTy",  &getType<&llvm::Type::getDoubleTy>);
        Nan::SetMethod(localObject, "getVoidTy",  &getType<&llvm::Type::getVoidTy>);
        Nan::SetMethod(localObject, "getLabelTy",  &getType<&llvm::Type::getLabelTy>);
        Nan::SetMethod(localObject, "getInt1Ty", &getIntType<&llvm::Type::getInt1Ty>);
        Nan::SetMethod(localObject, "getInt8Ty", &getIntType<&llvm::Type::getInt8Ty>);
        Nan::SetMethod(localObject, "getInt16Ty", &getIntType<&llvm::Type::getInt16Ty>);
        Nan::SetMethod(localObject, "getInt32Ty", &getIntType<&llvm::Type::getInt32Ty>);
        Nan::SetMethod(localObject, "getInt64Ty", &getIntType<&llvm::Type::getInt64Ty>);
        Nan::SetMethod(localObject, "getInt128Ty", &getIntType<&llvm::Type::getInt128Ty>);
        object.Reset(localObject);
    }

    return object;
}

bool TypeWrapper::isInstance(v8::Local<v8::Value> object) {
    return Nan::New(typeTemplate())->HasInstance(object);
}

llvm::Type *TypeWrapper::getType() {
    return type;
}

