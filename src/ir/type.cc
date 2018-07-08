//
// Created by Micha Reiser on 28.02.17.
//

#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/raw_ostream.h>
#include "type.h"
#include "pointer-type.h"

Napi::FunctionReference TypeWrapper::constructor;


typedef llvm::Type* (getTypeFn)(llvm::LLVMContext&);
template<getTypeFn method>
Napi::Value getTypeFactory(const Napi::CallbackInfo& info) {
    auto env = info.Env();
    if (info.Length() < 1 || !LLVMContextWrapper::isInstanceOfType(info[0])) {
        throw Napi::TypeError::New(env, "getType needs to be called with the context");
    }

    auto context = LLVMContextWrapper::Unwrap(info[0].As<Napi::Object>());

    auto* type = method(context->getContext());
    return TypeWrapper::of(env, type);
}

typedef llvm::IntegerType* (getIntTypeFn)(llvm::LLVMContext&);
template<getIntTypeFn method>
Napi::Value getIntType(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    if (info.Length() < 1 || !LLVMContextWrapper::isInstanceOfType(info[0])) {
        throw Napi::TypeError::New(env, "getIntTy needs to be called with the context");
    }

    auto context = LLVMContextWrapper::Unwrap(info[0].As<Napi::Object>());

    auto* type = method(context->getContext());
    return TypeWrapper::of(env, type);
}

typedef llvm::PointerType* (getPointerTypeFn)(llvm::LLVMContext&, unsigned AS);
template<getPointerTypeFn method>
Napi::Value getPointerType(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    if (info.Length() < 1 || !LLVMContextWrapper::isInstanceOfType(info[0]) ||
    (info.Length() == 2 && !info[1].IsNumber())) {
        throw Napi::TypeError::New(env, "getPointerTy needs to be called with: context: LLVMContext, AS=0: uint32");
    }

    auto context = LLVMContextWrapper::Unwrap(info[0].As<Napi::Object>());
    unsigned AS = 0;

    if (info.Length() == 2) {
        AS = info[1].As<Napi::Number>();
    }

    auto* type = method(context->getContext(), AS);
//    TODO uncomment when PointerType is migrated
//    return PointerTypeWrapper::of(type);
    return env.Undefined();
}

void TypeWrapper::Init(Napi::Env env, Napi::Object &exports){
    Napi::HandleScope scope { env };

    Napi::Object typeIds = Napi::Object::New(env);
    typeIds.Set("HalfTyID", Napi::Number::New(env, llvm::Type::TypeID::HalfTyID));
    typeIds.Set("VoidTyID", Napi::Number::New(env, llvm::Type::TypeID::VoidTyID));
    typeIds.Set("FloatTyID", Napi::Number::New(env, llvm::Type::TypeID::FloatTyID));
    typeIds.Set("DoubleTyID", Napi::Number::New(env, llvm::Type::TypeID::DoubleTyID));
    typeIds.Set("X86_FP80TyID", Napi::Number::New(env, llvm::Type::TypeID::X86_FP80TyID));
    typeIds.Set("FP128TyID", Napi::Number::New(env, llvm::Type::TypeID::FP128TyID));
    typeIds.Set("PPC_FP128TyID", Napi::Number::New(env, llvm::Type::TypeID::PPC_FP128TyID));
    typeIds.Set("LabelTyID", Napi::Number::New(env, llvm::Type::TypeID::LabelTyID));
    typeIds.Set("MetadataTyID", Napi::Number::New(env, llvm::Type::TypeID::MetadataTyID));
    typeIds.Set("X86_MMXTyID", Napi::Number::New(env, llvm::Type::TypeID::X86_MMXTyID));
    typeIds.Set("TokenTyID", Napi::Number::New(env, llvm::Type::TypeID::TokenTyID));
    typeIds.Set("IntegerTyID", Napi::Number::New(env, llvm::Type::TypeID::IntegerTyID));
    typeIds.Set("FunctionTyID", Napi::Number::New(env, llvm::Type::TypeID::FunctionTyID));
    typeIds.Set("StructTyID", Napi::Number::New(env, llvm::Type::TypeID::StructTyID));
    typeIds.Set("ArrayTyID", Napi::Number::New(env, llvm::Type::TypeID::ArrayTyID));
    typeIds.Set("PointerTyID", Napi::Number::New(env, llvm::Type::TypeID::PointerTyID));
    typeIds.Set("VectorTyID", Napi::Number::New(env, llvm::Type::TypeID::VectorTyID));

    Napi::Function func = DefineClass(env, "Type", {
        StaticValue("TypeID", typeIds),
        StaticMethod("getDoubleTy", &getTypeFactory<&llvm::Type::getDoubleTy>),
        StaticMethod("getVoidTy",  &getTypeFactory<&llvm::Type::getVoidTy>),
        StaticMethod("getFloatTy", &getTypeFactory<&llvm::Type::getFloatTy>),
        StaticMethod("getLabelTy",  &getTypeFactory<&llvm::Type::getLabelTy>),
        StaticMethod("getInt1Ty", &getIntType<&llvm::Type::getInt1Ty>),
        StaticMethod("getInt8Ty", &getIntType<&llvm::Type::getInt8Ty>),
        StaticMethod("getInt16Ty", &getIntType<&llvm::Type::getInt16Ty>),
        StaticMethod("getInt32Ty", &getIntType<&llvm::Type::getInt32Ty>),
        StaticMethod("getInt64Ty", &getIntType<&llvm::Type::getInt64Ty>),
        StaticMethod("getInt128Ty", &getIntType<&llvm::Type::getInt128Ty>),
        StaticMethod("getIntNTy", getIntNTy),
        StaticMethod("getInt1PtrTy", &getPointerType<&llvm::Type::getInt1PtrTy>),
        StaticMethod("getInt8PtrTy", &getPointerType<&llvm::Type::getInt8PtrTy>),
        StaticMethod("getInt32PtrTy", &getPointerType<&llvm::Type::getInt32PtrTy>),
        InstanceMethod("equals", &TypeWrapper::equals),
        InstanceMethod("isVoidTy", &TypeWrapper::isOfType<&llvm::Type::isVoidTy>),
        InstanceMethod("isFloatTy", &TypeWrapper::isOfType<&llvm::Type::isFloatTy>),
        InstanceMethod("isDoubleTy", &TypeWrapper::isOfType<&llvm::Type::isDoubleTy>),
        InstanceMethod("isLabelTy", &TypeWrapper::isOfType<&llvm::Type::isLabelTy>),
        InstanceMethod("isIntegerTy", &TypeWrapper::isIntegerTy),
        InstanceMethod("isFunctionTy", &TypeWrapper::isOfType<&llvm::Type::isFunctionTy>),
        InstanceMethod("isStructTy", &TypeWrapper::isOfType<&llvm::Type::isStructTy>),
        InstanceMethod("isArrayTy", &TypeWrapper::isOfType<&llvm::Type::isArrayTy>),
        InstanceMethod("isPointerTy", &TypeWrapper::isOfType<&llvm::Type::isPointerTy>),
        InstanceAccessor("typeID", &TypeWrapper::getTypeID, nullptr),
        InstanceMethod("getPointerTo", &TypeWrapper::getPointerTo),
        InstanceMethod("getPrimitiveSizeInBits", &TypeWrapper::getPrimitiveSizeInBits),
        InstanceMethod("toString", &TypeWrapper::toString)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Type", func);
}



Napi::Object TypeWrapper::of(Napi::Env env, llvm::Type *type) {
    Napi::Object result {};

//    TODO uncomment when time has come
//    if (type->isFunctionTy()) {
//        result = FunctionTypeWrapper::Create(static_cast<llvm::FunctionType*>(type));
//    }
    if (type->isPointerTy()) {
        result = PointerTypeWrapper::of(env, static_cast<llvm::PointerType*>(type));
//    else if (type->isArrayTy()) {
//        result = ArrayTypeWrapper::of(static_cast<llvm::ArrayType*>(type));
//    } else if (type->isStructTy()) {
//        result = StructTypeWrapper::of(static_cast<llvm::StructType*>(type));
    } else {
        result = constructor.New({ Napi::External<llvm::Type>::New(env, type) });
    }

    return result;
}

bool TypeWrapper::isInstanceOfType(const Napi::Value& value) {
    return value.As<Napi::Object>().InstanceOf(constructor.Value());
}

TypeWrapper::TypeWrapper(const Napi::CallbackInfo &info): ObjectWrap<TypeWrapper> { info } {
    auto env = info.Env();

    if (!info.IsConstructCall()) {
        throw Napi::TypeError::New(env, "Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0].IsExternal()) {
        throw Napi::TypeError::New(env, "Expected type pointer");
    }

    auto external = info[0].As<Napi::External<llvm::Type>>();
    type = external.Data();
}

Napi::Value TypeWrapper::equals(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    if (info.Length() != 1 || !TypeWrapper::isInstanceOfType(info[0])) {
        throw Napi::TypeError::New(env, "equals needs to be called with: other: Type");
    }

    llvm::Type* other = TypeWrapper::Unwrap(info[0].As<Napi::Object>())->getType();
    return Napi::Boolean::New(env, type == other);
}

Napi::Value TypeWrapper::getPointerTo(const Napi::CallbackInfo &info){
    auto env = info.Env();

    if ((info.Length() == 1 && !info[0].IsNumber()) || info.Length() > 1) {
        throw Napi::TypeError::New(env, "getPointer needs to called with: addrSpace?: uint32");
    }

    uint32_t addressSpace {};
    if (info.Length() == 1) {
        addressSpace = info[0].As<Napi::Number>();
    }

    auto* pointerType = type->getPointerTo(addressSpace);
    return PointerTypeWrapper::of(env, pointerType);
}

Napi::Value TypeWrapper::toString(const Napi::CallbackInfo &info) {
    std::string name {};
    llvm::raw_string_ostream output { name };

    type->print(output);

    return Napi::String::New(info.Env(), output.str());
}

Napi::Value TypeWrapper::getTypeID(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), type->getTypeID());
}

Napi::Value TypeWrapper::getIntNTy(const Napi::CallbackInfo &info){
    auto env = info.Env();

    if (info.Length() != 2 || !LLVMContextWrapper::isInstanceOfType(info[0]) || !info[1].IsNumber()) {
        throw Napi::TypeError::New(env, "getIntNTy needs to be called with: context: LLVMContext, N: uint32");
    }

    auto& context = LLVMContextWrapper::Unwrap(info[0].As<Napi::Object>())->getContext();
    auto N = info[1].As<Napi::Number>();
    auto* type = llvm::Type::getIntNTy(context, N);

    return TypeWrapper::of(env, type);
}

Napi::Value TypeWrapper::getPrimitiveSizeInBits(const Napi::CallbackInfo &info){
    return Napi::Number::New(info.Env(), type->getPrimitiveSizeInBits());
}


Napi::Value TypeWrapper::isIntegerTy(const Napi::CallbackInfo& info) {
    auto env = info.Env();

    if (info.Length() > 1 && info[0].IsNumber()) {
        throw Napi::TypeError::New(env, "isIntegerTy needs to be called with: bitwidth?: uint32");
    }

    bool result = info.Length() == 0 ? type->isIntegerTy() : type->isIntegerTy(info[0].As<Napi::Number>());
    return Napi::Boolean::New(env, result);
}


llvm::Type *TypeWrapper::getType() {
    return type;
}

