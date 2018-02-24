//
// Created by Micha Reiser on 01.03.17.
//

#include <llvm/IR/Function.h>
#include "constant.h"
//#include "type.h"
//#include "function.h"
//#include "pointer-type.h"
//#include "../util/array.h"
//#include "../util/string.h"
//#include "constant-fp.h"
//#include "constant-int.h"
//#include "constant-data-array.h"
//#include "constant-pointer-null.h"
//#include "global-variable.h"
//#include "constant-struct.h"
//#include "constant-array.h"
//#include "undef-value.h"

Napi::FunctionReference ConstantWrapper::constructor;

Napi::Object ConstantWrapper::Init(Napi::Env env, Napi::Object exports) {
    // TODO test inheritance
    Napi::Function func = DefineClass(env, "Constant", {
            StaticMethod("getNullValue", &ConstantWrapper::getNullValue),
            StaticMethod("getAllOnesValue", &ConstantWrapper::getAllOnesValue),
            InstanceMethod("isNullValue", &ConstantWrapper::isNullValue),
            InstanceMethod("isOneValue", &ConstantWrapper::isOneValue),
            InstanceMethod("isAllOnesValue", &ConstantWrapper::isAllOnesValue)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Constant", func);
    return exports;
}

Napi::Object ConstantWrapper::of(Napi::Env env, llvm::Constant *constant) {
    Napi::Object result;

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
    result = constructor.New({Napi::External<llvm::Constant>::New(env, constant)});
//    }

    Napi::EscapableHandleScope escapeScope{env};
    return escapeScope.Escape(result).ToObject();
}


ConstantWrapper::ConstantWrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<ConstantWrapper> {info} {
}

Napi::Value ConstantWrapper::getNullValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() != 1 || !TypeWrapper::isInstanceOfType(info[0])) {
        throw Napi::TypeError::New(env, "getNullValue needs to be called with: type: Type");
    }

    const llvm::Type *type = TypeWrapper::Unwrap(info[0].ToObject())->getType();
    llvm::Value *constant = llvm::Constant::getNullValue(type);

    return ConstantWrapper::of(env, constant);
}

Napi::Value ConstantWrapper::getAllOnesValue(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    if (info.Length() != 1 || !TypeWrapper::isInstanceOfType(info[0])) {
        throw Napi::TypeError::New(env, "getAllOnesValue needs to be called with: type: Type");
    }

    const llvm::Type *type = TypeWrapper::Unwrap(info[0].ToObject())->getType();
    llvm::Constant *constant = llvm::Constant::getAllOnesValue(type);

    return ConstantWrapper::of(env, constant);
}

Napi::Value ConstantWrapper::isNullValue(const Napi::CallbackInfo &info) {
    return Napi::Boolean::New(info.Env(), this->getConstant()->isNullValue());
}

Napi::Value ConstantWrapper::IsOneValue(const Napi::CallbackInfo &info) {
    return Napi::Boolean::New(info.Env(), this->getConstant()->isOneValue());
}

Napi::Value ConstantWrapper::isAllOnesValue(const Napi::CallbackInfo &info) {
    return Napi::Boolean::New(info.Env(), this->getConstant()->isAllOnesValue());
}

llvm::Constant *ConstantWrapper::getConstant() {
    return static_cast<llvm::Constant *>(getValue());
}