//
// Created by Micha Reiser on 28.02.17.
//

#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/raw_ostream.h>
#include "type.h"
#include "function-type.h"
#include "pointer-type.h"
#include "array-type.h"
#include "struct-type.h"

Napi::Object TypeWrapper::Init(Napi::Env env, Napi::Object exports) {
    auto type = Napi::GetFunction(Napi::New(env, typeTemplate()));

    auto typeIds = Napi::Object::New(env);
    (typeIds).Set(Napi::String::New(env, "VoidTyID"), Napi::New(env, llvm::Type::TypeID::VoidTyID));
    (typeIds).Set(Napi::String::New(env, "HalfTyID"), Napi::New(env, llvm::Type::TypeID::HalfTyID));
    (typeIds).Set(Napi::String::New(env, "FloatTyID"), Napi::New(env, llvm::Type::TypeID::FloatTyID));
    (typeIds).Set(Napi::String::New(env, "DoubleTyID"), Napi::New(env, llvm::Type::TypeID::DoubleTyID));
    (typeIds).Set(Napi::String::New(env, "X86_FP80TyID"), Napi::New(env, llvm::Type::TypeID::X86_FP80TyID));
    (typeIds).Set(Napi::String::New(env, "FP128TyID"), Napi::New(env, llvm::Type::TypeID::FP128TyID));
    (typeIds).Set(Napi::String::New(env, "PPC_FP128TyID"), Napi::New(env, llvm::Type::TypeID::PPC_FP128TyID));
    (typeIds).Set(Napi::String::New(env, "LabelTyID"), Napi::New(env, llvm::Type::TypeID::LabelTyID));
    (typeIds).Set(Napi::String::New(env, "MetadataTyID"), Napi::New(env, llvm::Type::TypeID::MetadataTyID));
    (typeIds).Set(Napi::String::New(env, "X86_MMXTyID"), Napi::New(env, llvm::Type::TypeID::X86_MMXTyID));
    (typeIds).Set(Napi::String::New(env, "TokenTyID"), Napi::New(env, llvm::Type::TypeID::TokenTyID));
    (typeIds).Set(Napi::String::New(env, "IntegerTyID"), Napi::New(env, llvm::Type::TypeID::IntegerTyID));
    (typeIds).Set(Napi::String::New(env, "FunctionTyID"), Napi::New(env, llvm::Type::TypeID::FunctionTyID));
    (typeIds).Set(Napi::String::New(env, "StructTyID"), Napi::New(env, llvm::Type::TypeID::StructTyID));
    (typeIds).Set(Napi::String::New(env, "ArrayTyID"), Napi::New(env, llvm::Type::TypeID::ArrayTyID));
    (typeIds).Set(Napi::String::New(env, "PointerTyID"), Napi::New(env, llvm::Type::TypeID::PointerTyID));
    (typeIds).Set(Napi::String::New(env, "VectorTyID"), Napi::New(env, llvm::Type::TypeID::VectorTyID));

    (type).Set(Napi::String::New(env, "TypeID"), typeIds);

    (target).Set(Napi::String::New(env, "Type"), type);
}

Napi::Value TypeWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "Constructor needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() < 1 || !info[0].IsExternal()) {
        Napi::TypeError::New(env, "Expected type pointer").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = static_cast<llvm::Type*>(*info[0].As<Napi::External>()->Value());
    auto* wrapper = new TypeWrapper { type };
    wrapper->Wrap(info.This());

    return info.This();
}

Napi::Value TypeWrapper::equals(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "equals needs to be called with: other: Type").ThrowAsJavaScriptException();
        return env.Null();
    }

    llvm::Type* that = TypeWrapper::FromValue(info.Holder())->getType();
    llvm::Type* other = TypeWrapper::FromValue(info[0])->getType();

    return Napi::New(env, that == other);
}

Napi::Value TypeWrapper::getPointerTo(const Napi::CallbackInfo& info) {
    if ((info.Length() == 1 && !info[0].IsUint32()) || info.Length() > 1) {
        Napi::TypeError::New(env, "getPointer needs to called with: addrSpace?: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    uint32_t addressSpace {};
    if (info.Length() == 1) {
        addressSpace = info[0].As<Napi::Number>().Uint32Value();
    }

    auto* pointerType = TypeWrapper::FromValue(info.Holder())->getType()->getPointerTo(addressSpace);
    return PointerTypeWrapper::of(pointerType);
}

Napi::Value TypeWrapper::toString(const Napi::CallbackInfo& info) {
    auto* type = TypeWrapper::FromValue(info.Holder())->getType();

    std::string name {};
    llvm::raw_string_ostream output { name };

    type->print(output);

    return Napi::New(env, output.str());
}


typedef llvm::Type* (getTypeFn)(llvm::LLVMContext&);
template<getTypeFn method>
Napi::Value getTypeFactory(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "getType needs to be called with the context").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto context = LLVMContextWrapper::FromValue(info[0]);

    auto* type = method(context->getContext());
    auto wrapped = TypeWrapper::of(type);
    return wrapped;
}

typedef llvm::IntegerType* (getIntTypeFn)(llvm::LLVMContext&);
template<getIntTypeFn method>
Napi::Value getIntType(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "getIntTy needs to be called with the context").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto context = LLVMContextWrapper::FromValue(info[0]);

    auto* type = method(context->getContext());
    auto wrapped = TypeWrapper::of(type);
    return wrapped;
}

typedef llvm::PointerType* (getPointerTypeFn)(llvm::LLVMContext&, unsigned AS);
template<getPointerTypeFn method>
Napi::Value getPointerType(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0]) ||
            (info.Length() == 2 && !info[1].IsUint32())) {
        Napi::TypeError::New(env, "getPointerTy needs to be called with: context: LLVMContext, AS=0: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto context = LLVMContextWrapper::FromValue(info[0]);
    unsigned AS = 0;

    if (info.Length() == 2) {
        AS = Napi::To<unsigned>(info[1]);
    }

    auto* type = method(context->getContext(), AS);
    auto wrapped = PointerTypeWrapper::of(type);
    return wrapped;
}

Napi::Object TypeWrapper::of(llvm::Type *type) {
    Napi::Object result {};

    if (type->isFunctionTy()) {
        result = FunctionTypeWrapper::Create(static_cast<llvm::FunctionType*>(type));
    } else if (type->isPointerTy()) {
        result = PointerTypeWrapper::of(static_cast<llvm::PointerType*>(type));
    } else if (type->isArrayTy()) {
        result = ArrayTypeWrapper::of(static_cast<llvm::ArrayType*>(type));
    } else if (type->isStructTy()) {
        result = StructTypeWrapper::of(static_cast<llvm::StructType*>(type));
    } else {
        Napi::FunctionReference functionTemplate = Napi::New(env, typeTemplate());
        auto constructorFunction = Napi::GetFunction(functionTemplate);
        Napi::Value argv[1] = { Napi::External::New(env, type) };
        result = Napi::NewInstance(constructorFunction, 1, argv);
    }

    Napi::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(result);
}

typedef bool (llvm::Type::*isTy)() const;
template<isTy method>
Napi::Value isOfType(const Napi::CallbackInfo& info) {
    auto* type = TypeWrapper::FromValue(info.Holder())->getType();
    auto result = Napi::New(env, (type->*method)());
    return result;
}

Napi::Value isIntegerTy(const Napi::CallbackInfo& info) {
    if (info.Length() > 1 && info[0].IsUint32()) {
        Napi::TypeError::New(env, "isIntegerTy needs to be called with: bitwidth?: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    llvm::Type* type = TypeWrapper::FromValue(info.Holder())->getType();
    bool result = info.Length() == 0 ? type->isIntegerTy() : type->isIntegerTy(info[0].As<Napi::Number>().Uint32Value());
    return Napi::New(env, result);
}

Napi::Value TypeWrapper::getTypeID(const Napi::CallbackInfo& info) {
    auto* wrapper = TypeWrapper::FromValue(info.Holder());
    auto result = Napi::New(env, wrapper->type->getTypeID());
    return result;
}

Napi::Value TypeWrapper::getIntNTy(const Napi::CallbackInfo& info) {
    if (info.Length() != 2 || !LLVMContextWrapper::isInstance(info[0]) || !info[1].IsUint32()) {
        Napi::TypeError::New(env, "getIntNTy needs to be called with: context: LLVMContext, N: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    auto N = info[1].As<Napi::Number>().Uint32Value();
    auto* type = llvm::Type::getIntNTy(context, N);

    return TypeWrapper::of(type);
}

Napi::Value TypeWrapper::getPrimitiveSizeInBits(const Napi::CallbackInfo& info) {
    auto* type = TypeWrapper::FromValue(info.Holder())->getType();
    return Napi::New(env, type->getPrimitiveSizeInBits());
}

Napi::FunctionReference& TypeWrapper::typeTemplate() {
    static Napi::FunctionReference persistentTemplate {};

    if (persistentTemplate.IsEmpty()) {
        Napi::FunctionReference typeTemplate = Napi::Function::New(env, TypeWrapper::New);

        typeTemplate->SetClassName(Napi::String::New(env, "Type"));


        Napi::SetMethod(typeTemplate, "getDoubleTy",  &getTypeFactory<&llvm::Type::getDoubleTy>);
        Napi::SetMethod(typeTemplate, "getVoidTy",  &getTypeFactory<&llvm::Type::getVoidTy>);
        Napi::SetMethod(typeTemplate, "getFloatTy", &getTypeFactory<&llvm::Type::getFloatTy>);
        Napi::SetMethod(typeTemplate, "getLabelTy",  &getTypeFactory<&llvm::Type::getLabelTy>);
        Napi::SetMethod(typeTemplate, "getInt1Ty", &getIntType<&llvm::Type::getInt1Ty>);
        Napi::SetMethod(typeTemplate, "getInt8Ty", &getIntType<&llvm::Type::getInt8Ty>);
        Napi::SetMethod(typeTemplate, "getInt16Ty", &getIntType<&llvm::Type::getInt16Ty>);
        Napi::SetMethod(typeTemplate, "getInt32Ty", &getIntType<&llvm::Type::getInt32Ty>);
        Napi::SetMethod(typeTemplate, "getInt64Ty", &getIntType<&llvm::Type::getInt64Ty>);
        Napi::SetMethod(typeTemplate, "getInt128Ty", &getIntType<&llvm::Type::getInt128Ty>);
        Napi::SetMethod(typeTemplate, "getIntNTy", getIntNTy);
        Napi::SetMethod(typeTemplate, "getInt1PtrTy", &getPointerType<&llvm::Type::getInt1PtrTy>);
        Napi::SetMethod(typeTemplate, "getInt8PtrTy", &getPointerType<&llvm::Type::getInt8PtrTy>);
        Napi::SetMethod(typeTemplate, "getInt32PtrTy", &getPointerType<&llvm::Type::getInt32PtrTy>);

        Napi::SetPrototypeMethod(typeTemplate, "equals", &equals);
        Napi::SetPrototypeMethod(typeTemplate, "isVoidTy", &isOfType<&llvm::Type::isVoidTy>);
        Napi::SetPrototypeMethod(typeTemplate, "isFloatTy", &isOfType<&llvm::Type::isFloatTy>);
        Napi::SetPrototypeMethod(typeTemplate, "isDoubleTy", &isOfType<&llvm::Type::isDoubleTy>);
        Napi::SetPrototypeMethod(typeTemplate, "isLabelTy", &isOfType<&llvm::Type::isLabelTy>);
        Napi::SetPrototypeMethod(typeTemplate, "isIntegerTy", &isIntegerTy);
        Napi::SetPrototypeMethod(typeTemplate, "isFunctionTy", &isOfType<&llvm::Type::isFunctionTy>);
        Napi::SetPrototypeMethod(typeTemplate, "isStructTy", &isOfType<&llvm::Type::isStructTy>);
        Napi::SetPrototypeMethod(typeTemplate, "isArrayTy", &isOfType<&llvm::Type::isArrayTy>);
        Napi::SetPrototypeMethod(typeTemplate, "isPointerTy", &isOfType<&llvm::Type::isPointerTy>);
        Napi::SetAccessor(typeTemplate->InstanceTemplate(), Napi::String::New(env, "typeID"), TypeWrapper::getTypeID);
        Napi::SetPrototypeMethod(typeTemplate, "getPointerTo", TypeWrapper::getPointerTo);
        Napi::SetPrototypeMethod(typeTemplate, "getPrimitiveSizeInBits", TypeWrapper::getPrimitiveSizeInBits);
        Napi::SetPrototypeMethod(typeTemplate, "toString", TypeWrapper::toString);

        persistentTemplate.Reset(typeTemplate);
    }

    return persistentTemplate;
}

bool TypeWrapper::isInstance(Napi::Value object) {
    Napi::Env env = object.Env();
    return Napi::New(env, typeTemplate())->HasInstance(object);
}

llvm::Type *TypeWrapper::getType() {
    return type;
}

