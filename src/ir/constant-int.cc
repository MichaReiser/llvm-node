//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-int.h"
#include "llvm-context.h"
#include "../util/string.h"

NAN_MODULE_INIT(ConstantIntWrapper::Init) {
    auto constantInt = Nan::GetFunction(Nan::New(constantIntTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("ConstantInt").ToLocalChecked(), constantInt);
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
    if (info.Length() < 2 || !LLVMContextWrapper::isInstance(info[0]) || (!info[1]->IsNumber() && !info[1]->IsString()) ||
            (info.Length() > 2 && !info[2]->IsNumber() && !info[2]->IsUndefined()) ||
            (info.Length() > 3 && !info[3]->IsBoolean()) ||
            info.Length() > 4) {
        return Nan::ThrowTypeError("get needs to be called with: context: LLVMContext, value: number|string, numBits = 32, signed= true");
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    uint32_t numBits = 32;
    bool isSigned = true;

    if (info.Length() > 2 && !info[2]->IsUndefined()) {
        numBits = Nan::To<uint32_t>(info[2]).FromJust();
    }

    if (info.Length() == 3) {
        isSigned = Nan::To<bool>(info[3]).FromJust();
    }

    if (info[1]->IsString()) {
        auto number = ToString(Nan::To<v8::String>(info[1]).ToLocalChecked());
        auto* constant = llvm::ConstantInt::get(context, llvm::APInt { numBits, llvm::StringRef(number.c_str()), 10 } );

        info.GetReturnValue().Set(ConstantIntWrapper::of(constant));
    } else {
        int64_t number = Nan::To<int64_t>(info[1]).FromJust();
        auto* constant = llvm::ConstantInt::get(context, llvm::APInt { numBits, static_cast<uint64_t>(number), isSigned } );

        info.GetReturnValue().Set(ConstantIntWrapper::of(constant));
    }
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

NAN_METHOD(ConstantIntWrapper::toString) {
    auto* wrapper = ConstantIntWrapper::FromValue(info.Holder());
    auto constantInt = wrapper->getConstantInt();
    auto value = constantInt->getValue();

    info.GetReturnValue().Set(Nan::New<v8::String>(value.toString(10, true)).ToLocalChecked());
}

NAN_GETTER(ConstantIntWrapper::getValueApf) {
    auto* wrapper = ConstantIntWrapper::FromValue(info.Holder());
    auto constantInt = wrapper->getConstantInt();
    auto value = constantInt->getValue();

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

        Nan::SetMethod(localTemplate, "get", ConstantIntWrapper::get);
        Nan::SetMethod(localTemplate, "getFalse", ConstantIntWrapper::getFalse);
        Nan::SetMethod(localTemplate, "getTrue", ConstantIntWrapper::getTrue);
        Nan::SetPrototypeMethod(localTemplate, "toString", ConstantIntWrapper::toString);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("value").ToLocalChecked(), ConstantIntWrapper::getValueApf);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}