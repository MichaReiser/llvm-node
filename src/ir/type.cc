//
// Created by Micha Reiser on 28.02.17.
//

#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/raw_ostream.h>
#include "type.h"
#include "function-type.h"
#include "pointer-type.h"
#include "integer-type.h"
#include "array-type.h"
#include "struct-type.h"

NAN_MODULE_INIT(TypeWrapper::Init)
{
    auto type = Nan::GetFunction(Nan::New(typeTemplate())).ToLocalChecked();

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

    Nan::Set(type, Nan::New("TypeID").ToLocalChecked(), typeIds);

    Nan::Set(target, Nan::New("Type").ToLocalChecked(), type);
}

NAN_METHOD(TypeWrapper::New)
{
    if (!info.IsConstructCall())
    {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal())
    {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto *type = static_cast<llvm::Type *>(v8::External::Cast(*info[0])->Value());
    auto *wrapper = new TypeWrapper{type};
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(TypeWrapper::equals)
{
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0]))
    {
        return Nan::ThrowTypeError("equals needs to be called with: other: Type");
    }

    llvm::Type *that = TypeWrapper::FromValue(info.Holder())->getType();
    llvm::Type *other = TypeWrapper::FromValue(info[0])->getType();

    info.GetReturnValue().Set(Nan::New(that == other));
}

NAN_METHOD(TypeWrapper::getPointerTo)
{
    if ((info.Length() == 1 && !info[0]->IsUint32()) || info.Length() > 1)
    {
        return Nan::ThrowTypeError("getPointer needs to called with: addrSpace?: uint32");
    }

    uint32_t addressSpace{};
    if (info.Length() == 1)
    {
        addressSpace = Nan::To<uint32_t>(info[0]).FromJust();
    }

    auto *pointerType = TypeWrapper::FromValue(info.Holder())->getType()->getPointerTo(addressSpace);
    info.GetReturnValue().Set(PointerTypeWrapper::of(pointerType));
}

NAN_METHOD(TypeWrapper::toString)
{
    auto *type = TypeWrapper::FromValue(info.Holder())->getType();

    std::string name{};
    llvm::raw_string_ostream output{name};

    type->print(output);

    info.GetReturnValue().Set(Nan::New(output.str()).ToLocalChecked());
}

typedef llvm::Type *(getTypeFn)(llvm::LLVMContext &);
template <getTypeFn method>
NAN_METHOD(getTypeFactory)
{
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0]))
    {
        return Nan::ThrowTypeError("getType needs to be called with the context");
    }

    auto context = LLVMContextWrapper::FromValue(info[0]);

    auto *type = method(context->getContext());
    auto wrapped = TypeWrapper::of(type);
    info.GetReturnValue().Set(wrapped);
}

typedef llvm::IntegerType *(getIntTypeFn)(llvm::LLVMContext &);
template <getIntTypeFn method>
NAN_METHOD(getIntType)
{
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("getIntTy needs to be called with the context");
    }

    auto context = LLVMContextWrapper::FromValue(info[0]);

    auto *type = method(context->getContext());
    info.GetReturnValue().Set(IntegerTypeWrapper::of(type));
}

typedef llvm::PointerType *(getPointerTypeFn)(llvm::LLVMContext &, unsigned AS);
template <getPointerTypeFn method>
NAN_METHOD(getPointerType)
{
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0]) ||
        (info.Length() == 2 && !info[1]->IsUint32()))
    {
        return Nan::ThrowTypeError("getPointerTy needs to be called with: context: LLVMContext, AS=0: uint32");
    }

    auto context = LLVMContextWrapper::FromValue(info[0]);
    unsigned AS = 0;

    if (info.Length() == 2)
    {
        AS = Nan::To<unsigned>(info[1]).FromJust();
    }

    auto *type = method(context->getContext(), AS);
    auto wrapped = PointerTypeWrapper::of(type);
    info.GetReturnValue().Set(wrapped);
}

v8::Local<v8::Object> TypeWrapper::of(llvm::Type *type)
{
    v8::Local<v8::Object> result{};

    if (type->isIntegerTy()) {
        result = IntegerTypeWrapper::of(static_cast<llvm::IntegerType *>(type));
    } else if (type->isFunctionTy()) {
        result = FunctionTypeWrapper::Create(static_cast<llvm::FunctionType *>(type));
    } else if (type->isPointerTy()) {
        result = PointerTypeWrapper::of(static_cast<llvm::PointerType *>(type));
    } else if (type->isArrayTy()) {
        result = ArrayTypeWrapper::of(static_cast<llvm::ArrayType *>(type));
    } else if (type->isStructTy()) {
        result = StructTypeWrapper::of(static_cast<llvm::StructType *>(type));
    } else {
        v8::Local<v8::FunctionTemplate> functionTemplate = Nan::New(typeTemplate());
        auto constructorFunction = Nan::GetFunction(functionTemplate).ToLocalChecked();
        v8::Local<v8::Value> argv[1] = {Nan::New<v8::External>(type)};
        result = Nan::NewInstance(constructorFunction, 1, argv).ToLocalChecked();
    }

    Nan::EscapableHandleScope escapeScope{};
    return escapeScope.Escape(result);
}

typedef bool (llvm::Type::*isTy)() const;
template <isTy method>
NAN_METHOD(isOfType)
{
    auto *type = TypeWrapper::FromValue(info.Holder())->getType();
    auto result = Nan::New((type->*method)());
    info.GetReturnValue().Set(result);
}

NAN_METHOD(isIntegerTy)
{
    if (info.Length() > 1 && info[0]->IsUint32())
    {
        return Nan::ThrowTypeError("isIntegerTy needs to be called with: bitwidth?: uint32");
    }

    llvm::Type *type = TypeWrapper::FromValue(info.Holder())->getType();
    bool result = info.Length() == 0 ? type->isIntegerTy() : type->isIntegerTy(Nan::To<uint32_t>(info[0]).FromJust());
    info.GetReturnValue().Set(Nan::New(result));
}

NAN_GETTER(TypeWrapper::getTypeID)
{
    auto *wrapper = TypeWrapper::FromValue(info.Holder());
    auto result = Nan::New(wrapper->type->getTypeID());
    info.GetReturnValue().Set(result);
}

NAN_METHOD(TypeWrapper::getIntNTy)
{
    if (info.Length() != 2 || !LLVMContextWrapper::isInstance(info[0]) || !info[1]->IsUint32())
    {
        return Nan::ThrowTypeError("getIntNTy needs to be called with: context: LLVMContext, N: uint32");
    }

    auto &context = LLVMContextWrapper::FromValue(info[0])->getContext();
    auto N = Nan::To<uint32_t>(info[1]).FromJust();
    auto *type = llvm::Type::getIntNTy(context, N);

    info.GetReturnValue().Set(TypeWrapper::of(type));
}

NAN_METHOD(TypeWrapper::getPrimitiveSizeInBits)
{
    auto *type = TypeWrapper::FromValue(info.Holder())->getType();
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(static_cast<uint32_t>(type->getPrimitiveSizeInBits())));
}

Nan::Persistent<v8::FunctionTemplate> &TypeWrapper::typeTemplate()
{
    static Nan::Persistent<v8::FunctionTemplate> persistentTemplate{};

    if (persistentTemplate.IsEmpty())
    {
        v8::Local<v8::FunctionTemplate> typeTemplate = Nan::New<v8::FunctionTemplate>(TypeWrapper::New);

        typeTemplate->SetClassName(Nan::New("Type").ToLocalChecked());
        typeTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetMethod(typeTemplate, "getDoubleTy", &getTypeFactory<&llvm::Type::getDoubleTy>);
        Nan::SetMethod(typeTemplate, "getFP128Ty", &getTypeFactory<&llvm::Type::getFP128Ty>);
        Nan::SetMethod(typeTemplate, "getVoidTy", &getTypeFactory<&llvm::Type::getVoidTy>);
        Nan::SetMethod(typeTemplate, "getFloatTy", &getTypeFactory<&llvm::Type::getFloatTy>);
        Nan::SetMethod(typeTemplate, "getLabelTy", &getTypeFactory<&llvm::Type::getLabelTy>);
        Nan::SetMethod(typeTemplate, "getHalfTy", &getTypeFactory<&llvm::Type::getHalfTy>);
        Nan::SetMethod(typeTemplate, "getInt1Ty", &getIntType<&llvm::Type::getInt1Ty>);
        Nan::SetMethod(typeTemplate, "getInt8Ty", &getIntType<&llvm::Type::getInt8Ty>);
        Nan::SetMethod(typeTemplate, "getInt16Ty", &getIntType<&llvm::Type::getInt16Ty>);
        Nan::SetMethod(typeTemplate, "getInt32Ty", &getIntType<&llvm::Type::getInt32Ty>);
        Nan::SetMethod(typeTemplate, "getInt64Ty", &getIntType<&llvm::Type::getInt64Ty>);
        Nan::SetMethod(typeTemplate, "getInt128Ty", &getIntType<&llvm::Type::getInt128Ty>);
        Nan::SetMethod(typeTemplate, "getIntNTy", getIntNTy);
        Nan::SetMethod(typeTemplate, "getInt1PtrTy", &getPointerType<&llvm::Type::getInt1PtrTy>);
        Nan::SetMethod(typeTemplate, "getInt8PtrTy", &getPointerType<&llvm::Type::getInt8PtrTy>);
        Nan::SetMethod(typeTemplate, "getInt32PtrTy", &getPointerType<&llvm::Type::getInt32PtrTy>);
        Nan::SetMethod(typeTemplate, "getFloatPtrTy", &getPointerType<&llvm::Type::getFloatPtrTy>);
        Nan::SetMethod(typeTemplate, "getDoublePtrTy", &getPointerType<&llvm::Type::getDoublePtrTy>);

        Nan::SetPrototypeMethod(typeTemplate, "equals", &equals);
        Nan::SetPrototypeMethod(typeTemplate, "isVoidTy", &isOfType<&llvm::Type::isVoidTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isFloatTy", &isOfType<&llvm::Type::isFloatTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isDoubleTy", &isOfType<&llvm::Type::isDoubleTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isFP128Ty", &isOfType<&llvm::Type::isFP128Ty>);
        Nan::SetPrototypeMethod(typeTemplate, "isLabelTy", &isOfType<&llvm::Type::isLabelTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isIntegerTy", &isIntegerTy);
        Nan::SetPrototypeMethod(typeTemplate, "isFunctionTy", &isOfType<&llvm::Type::isFunctionTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isStructTy", &isOfType<&llvm::Type::isStructTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isArrayTy", &isOfType<&llvm::Type::isArrayTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isPointerTy", &isOfType<&llvm::Type::isPointerTy>);
        Nan::SetPrototypeMethod(typeTemplate, "isHalfTy", &isOfType<&llvm::Type::isHalfTy>);
        Nan::SetAccessor(typeTemplate->InstanceTemplate(), Nan::New("typeID").ToLocalChecked(), TypeWrapper::getTypeID);
        Nan::SetPrototypeMethod(typeTemplate, "getPointerTo", TypeWrapper::getPointerTo);
        Nan::SetPrototypeMethod(typeTemplate, "getPrimitiveSizeInBits", TypeWrapper::getPrimitiveSizeInBits);
        Nan::SetPrototypeMethod(typeTemplate, "toString", TypeWrapper::toString);

        persistentTemplate.Reset(typeTemplate);
    }

    return persistentTemplate;
}

bool TypeWrapper::isInstance(v8::Local<v8::Value> object)
{
    return Nan::New(typeTemplate())->HasInstance(object);
}

llvm::Type *TypeWrapper::getType()
{
    return type;
}
