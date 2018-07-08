//
// Created by Micha Reiser on 01.03.17.
//

#include <llvm/IR/Function.h>
#include "constant.h"

Napi::FunctionReference ConstantWrapper::constructor;

void ConstantWrapper::Init(Napi::Env env, Napi::Object &exports){
    Napi::HandleScope scope { env };

    Napi::Function func = DefineClass(env, "Constant", {
        StaticMethod("getNullValue", ConstantWrapper::getNullValue),
        StaticMethod("getAllOnesValue", ConstantWrapper::getAllOnesValue),
        InstanceMethod("isNullValue", &ConstantWrapper::isNullValue),
        InstanceMethod("isOneValue", &ConstantWrapper::isOneValue),
        InstanceMethod("isAllOnesValue", &ConstantWrapper::isAllOnesValue)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Constant", func);
}

ConstantWrapper::ConstantWrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<ConstantWrapper> { info } {
    auto env = info.Env();

    if (!info.IsConstructCall()) {
        throw Napi::TypeError::New(env, "Class Constructor Constant cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0].IsExternal()) {
        throw Napi::TypeError::New(env, "Constant constructor requires: constant: External");
    }

    auto external = info[0].As<Napi::External<llvm::Constant>>();
    constant = external.Data();
}

Napi::Object ConstantWrapper::of(Napi::Env env, llvm::Constant *constant) {
    Napi::Object result;

//    TODO uncomment when time has come
//    if (llvm::Function::classof(constant)) {
//        result = FunctionWrapper::of(static_cast<llvm::Function*>(constant));
//    } else if (llvm::ConstantFP::classof(constant)) {
//        result = ConstantFPWrapper::of(static_cast<llvm::ConstantFP*>(constant));
//    } else if (llvm::ConstantInt::classof(constant)) {
//        result = ConstantIntWrapper::of(static_cast<llvm::ConstantInt*>(constant));
//    } else if (llvm::ConstantPointerNull::classof(constant)) {
//        result = ConstantPointerNullWrapper::of(static_cast<llvm::ConstantPointerNull*>(constant));
//    } else if (llvm::ConstantDataArray::classof(constant)) {
//        result = ConstantDataArrayWrapper::of(static_cast<llvm::ConstantDataArray*>(constant));
//    } else if (llvm::GlobalVariable::classof(constant)) {
//        result = GlobalVariableWrapper::of(static_cast<llvm::GlobalVariable*>(constant));
//    } else if (llvm::ConstantStruct::classof(constant)) {
//        result = ConstantStructWrapper::of(static_cast<llvm::ConstantStruct*>(constant));
//    } else if (llvm::ConstantArray::classof(constant)) {
//        result = ConstantArrayWrapper::of(static_cast<llvm::ConstantArray*>(constant));
//    } else if (llvm::UndefValue::classof(constant)) {
//        result = UndefValueWrapper::of(static_cast<llvm::UndefValue*>(constant));
//    } else {
        result = ConstantWrapper::constructor.New({ Napi::External<llvm::Constant>::New(env, constant)});
//    }

    return result;
}

Napi::Value ConstantWrapper::getNullValue(const Napi::CallbackInfo &info){
//    TODO uncomment when type wrapper is migrated
//    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
//        return Nan::ThrowTypeError("getNullValue needs to be called with: type: Type");
//    }

//    auto* type = TypeWrapper::FromValue(info[0])->getType();
//    auto* constant = llvm::Constant::getNullValue(type);
//    return ConstantWrapper::of(info.Env(), constant):

    return info.Env().Undefined();
}

Napi::Value ConstantWrapper::getAllOnesValue(const Napi::CallbackInfo &info){
//    TODO uncomment when type wrapper is migrated
//    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
//        return Nan::ThrowTypeError("getAllOnesValue needs to be called with: type: Type");
//    }
//
//    auto* type = TypeWrapper::FromValue(info[0])->getType();
//    auto* constant = llvm::Constant::getAllOnesValue(type);
//    return ConstantWrapper::of(info.Env(), constant);
    return info.Env().Undefined();
}

Napi::Value ConstantWrapper::isNullValue(const Napi::CallbackInfo &info){
    auto env = info.Env();

    return Napi::Boolean::New(env, constant->isNullValue());
}

Napi::Value ConstantWrapper::isOneValue(const Napi::CallbackInfo &info){
    auto env = info.Env();

    return Napi::Boolean::New(env, constant->isOneValue());
}

Napi::Value ConstantWrapper::isAllOnesValue(const Napi::CallbackInfo &info){
    auto env = info.Env();

    return Napi::Boolean::New(env, constant->isAllOnesValue());
}


llvm::Constant *ConstantWrapper::getConstant() {
    return constant;
}