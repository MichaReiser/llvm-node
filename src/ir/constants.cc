//
// Created by Micha Reiser on 01.03.17.
//

#include <llvm/IR/Function.h>
#include "constants.h"
#include "type.h"
#include "function.h"

NAN_MODULE_INIT(ConstantWrapper::Init) {
    v8::Local<v8::Object> object = Nan::New<v8::Object>();

    Nan::SetMethod(object, "getNullValue", ConstantWrapper::getNullValue);
    Nan::SetMethod(object, "getAllOnesValue", ConstantWrapper::getAllOnesValue);

    Nan::Set(target, Nan::New("Constant").ToLocalChecked(), object);
}

v8::Local<v8::Object> ConstantWrapper::of(llvm::Constant *constant) {
    v8::Local<v8::Object> result;

    if (llvm::Function::classof(constant)) {
        result = FunctionWrapper::of(static_cast<llvm::Function*>(constant));
    } else if (llvm::ConstantFP::classof(constant)) {
        result = ConstantFPWrapper::of(static_cast<llvm::ConstantFP*>(constant));
    } else if (llvm::ConstantInt::classof(constant)) {
        result = ConstantIntWrapper::of(static_cast<llvm::ConstantInt*>(constant));
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

//---------------------------------------------
// Constant FP
//---------------------------------------------

NAN_MODULE_INIT(ConstantFPWrapper::Init) {
    auto object = Nan::New<v8::Object>();

    Nan::SetMethod(object, "get", ConstantFPWrapper::get);

    Nan::Set(target, Nan::New("ConstantFP").ToLocalChecked(), object);
}

NAN_METHOD(ConstantFPWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor ConstantFP cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("ConstantFP constructor needs to be called with: constantFP: external");
    }

    auto* constantFp = static_cast<llvm::ConstantFP*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ConstantFPWrapper { constantFp };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ConstantFPWrapper::get) {
    if (info.Length() != 2 || !LLVMContextWrapper::isInstance(info[0]) || !info[1]->IsNumber()) {
        return Nan::ThrowTypeError("get needs to be called with: context: LLVMContext, value: number");
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    double number = Nan::To<double>(info[1]).ToChecked();

    auto* constant = llvm::ConstantFP::get(context, llvm::APFloat { number } );

    info.GetReturnValue().Set(ConstantFPWrapper::of(constant));
}

NAN_GETTER(ConstantFPWrapper::getValueAPF) {
    auto* wrapper = ConstantFPWrapper::FromValue(info.Holder());
    auto value = wrapper->getConstantFP()->getValueAPF();

    info.GetReturnValue().Set(Nan::New(value.convertToDouble()));
}

llvm::ConstantFP *ConstantFPWrapper::getConstantFP() {
    return static_cast<llvm::ConstantFP*>(getValue());
}

v8::Local<v8::Object> ConstantFPWrapper::of(llvm::ConstantFP *constantFP) {
    auto constructorFunction = Nan::GetFunction(Nan::New(constantFpTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(constantFP) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe {};
    return escapeScpoe.Escape(instance);
}

Nan::Persistent<v8::FunctionTemplate>& ConstantFPWrapper::constantFpTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(ConstantFPWrapper::New);
        localTemplate->SetClassName(Nan::New("ConstantFP").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(constantTemplate()));

        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("value").ToLocalChecked(), ConstantFPWrapper::getValueAPF);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

//---------------------------------------------
// Constant Int
//---------------------------------------------

NAN_MODULE_INIT(ConstantIntWrapper::Init) {
    auto object = Nan::New<v8::Object>();

    Nan::SetMethod(object, "get", ConstantIntWrapper::get);
    Nan::SetMethod(object, "getFalse", ConstantIntWrapper::getFalse);
    Nan::SetMethod(object, "getTrue", ConstantIntWrapper::getTrue);

    Nan::Set(target, Nan::New("ConstantInt").ToLocalChecked(), object);
}

NAN_METHOD(ConstantIntWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor ConstantInt cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("ConstantInt constructor needs to be called with: constantInt: external");
    }

    auto* constantInt = static_cast<llvm::ConstantInt*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ConstantIntWrapper { constantInt };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ConstantIntWrapper::get) {
    if (info.Length() != 2 || !LLVMContextWrapper::isInstance(info[0]) || !info[1]->IsInt32()) {
        return Nan::ThrowTypeError("get needs to be called with: context: LLVMContext, value: number");
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    int32_t number = Nan::To<int32_t >(info[1]).ToChecked();

    auto* constant = llvm::ConstantInt::get(context, llvm::APInt { 32, static_cast<uint64_t>(number), true } );

    info.GetReturnValue().Set(ConstantIntWrapper::of(constant));
}

NAN_METHOD(ConstantIntWrapper::getTrue) {
    if (info.Length() != 1 || !LLVMContextWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("getTrue needs to be called with: context: LLVMContext");
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    auto* constant = llvm::ConstantInt::getTrue(context);

    info.GetReturnValue().Set(ConstantIntWrapper::of(constant));
}

NAN_METHOD(ConstantIntWrapper::getFalse) {
    if (info.Length() != 1 || !LLVMContextWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("getFalse needs to be called with: context: LLVMContext");
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    auto* constant = llvm::ConstantInt::getFalse(context);

    info.GetReturnValue().Set(ConstantIntWrapper::of(constant));
}

NAN_GETTER(ConstantIntWrapper::getValueApf) {
    auto* wrapper = ConstantIntWrapper::FromValue(info.Holder());
    auto value = wrapper->getConstantInt()->getValue();

    info.GetReturnValue().Set(Nan::New(value.signedRoundToDouble()));
}

llvm::ConstantInt *ConstantIntWrapper::getConstantInt() {
    return static_cast<llvm::ConstantInt*>(getValue());
}

v8::Local<v8::Object> ConstantIntWrapper::of(llvm::ConstantInt *constantInt) {
    auto constructorFunction = Nan::GetFunction(Nan::New(constantIntTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(constantInt) };
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScpoe {};
    return escapeScpoe.Escape(instance);
}

Nan::Persistent<v8::FunctionTemplate>& ConstantIntWrapper::constantIntTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(ConstantIntWrapper::New);
        localTemplate->SetClassName(Nan::New("ConstantInt").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(constantTemplate()));

        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("value").ToLocalChecked(), ConstantIntWrapper::getValueApf);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}