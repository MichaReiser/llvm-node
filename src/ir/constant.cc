//
// Created by Micha Reiser on 01.03.17.
//

#include <llvm/IR/Function.h>
#include "constant.h"
#include "type.h"
#include "function.h"
#include "pointer-type.h"
#include "../util/array.h"
#include "../util/string.h"
#include "constant-fp.h"
#include "constant-int.h"
#include "constant-data-array.h"
#include "constant-pointer-null.h"
#include "global-variable.h"
#include "constant-struct.h"
#include "constant-array.h"
#include "undef-value.h"

NAN_MODULE_INIT(ConstantWrapper::Init) {
    auto constant = Nan::GetFunction(Nan::New(constantTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("Constant").ToLocalChecked(), constant);
}

v8::Local<v8::Object> ConstantWrapper::of(llvm::Constant *constant) {
    v8::Local<v8::Object> result;

    if (llvm::Function::classof(constant)) {
        result = FunctionWrapper::of(static_cast<llvm::Function*>(constant));
    } else if (llvm::ConstantFP::classof(constant)) {
        result = ConstantFPWrapper::of(static_cast<llvm::ConstantFP*>(constant));
    } else if (llvm::ConstantInt::classof(constant)) {
        result = ConstantIntWrapper::of(static_cast<llvm::ConstantInt*>(constant));
    } else if (llvm::ConstantPointerNull::classof(constant)) {
        result = ConstantPointerNullWrapper::of(static_cast<llvm::ConstantPointerNull*>(constant));
    } else if (llvm::ConstantDataArray::classof(constant)) {
        result = ConstantDataArrayWrapper::of(static_cast<llvm::ConstantDataArray*>(constant));
    } else if (llvm::GlobalVariable::classof(constant)) {
        result = GlobalVariableWrapper::of(static_cast<llvm::GlobalVariable*>(constant));
    } else if (llvm::ConstantStruct::classof(constant)) {
        result = ConstantStructWrapper::of(static_cast<llvm::ConstantStruct*>(constant));
    } else if (llvm::ConstantArray::classof(constant)) {
        result = ConstantArrayWrapper::of(static_cast<llvm::ConstantArray*>(constant));
    } else if (llvm::UndefValue::classof(constant)) {
        result = UndefValueWrapper::of(static_cast<llvm::UndefValue*>(constant));
    } else {
        auto constructorFunction = Nan::GetFunction(Nan::New(constantTemplate())).ToLocalChecked();
        v8::Local<v8::Value> argv[1] = { Nan::New<v8::External>(constant) };
        result = Nan::NewInstance(constructorFunction, 1, argv).ToLocalChecked();
    }

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(result);
}

NAN_METHOD(ConstantWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor Constant cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Constant constructor requires: constant: External");
    }

    auto* constant = static_cast<llvm::Constant*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ConstantWrapper { constant };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ConstantWrapper::getNullValue) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("getNullValue needs to be called with: type: Type");
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto* constant = llvm::Constant::getNullValue(type);

    info.GetReturnValue().Set(ConstantWrapper::of(constant));
}

NAN_METHOD(ConstantWrapper::getAllOnesValue) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("getAllOnesValue needs to be called with: type: Type");
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto* constant = llvm::Constant::getAllOnesValue(type);

    info.GetReturnValue().Set(ConstantWrapper::of(constant));
}

NAN_METHOD(ConstantWrapper::isNullValue) {
    auto* wrapper = ConstantWrapper::FromValue(info.Holder());
    info.GetReturnValue().Set(Nan::New(wrapper->getConstant()->isNullValue()));
}

NAN_METHOD(ConstantWrapper::isOneValue) {
    auto* wrapper = ConstantWrapper::FromValue(info.Holder());
    info.GetReturnValue().Set(Nan::New(wrapper->getConstant()->isOneValue()));
}

NAN_METHOD(ConstantWrapper::isAllOnesValue) {
    auto* wrapper = ConstantWrapper::FromValue(info.Holder());
    info.GetReturnValue().Set(Nan::New(wrapper->getConstant()->isAllOnesValue()));
}

Nan::Persistent<v8::FunctionTemplate>& ConstantWrapper::constantTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(ConstantWrapper::New);
        localTemplate->SetClassName(Nan::New("Constant").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(ValueWrapper::valueTemplate()));

        Nan::SetMethod(localTemplate, "getNullValue", ConstantWrapper::getNullValue);
        Nan::SetMethod(localTemplate, "getAllOnesValue", ConstantWrapper::getAllOnesValue);
        Nan::SetPrototypeMethod(localTemplate, "isNullValue", ConstantWrapper::isNullValue);
        Nan::SetPrototypeMethod(localTemplate, "isOneValue", ConstantWrapper::isOneValue);
        Nan::SetPrototypeMethod(localTemplate, "isAllOnesValue", ConstantWrapper::isAllOnesValue);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

llvm::Constant *ConstantWrapper::getConstant() {
    return static_cast<llvm::Constant*>(getValue());
}