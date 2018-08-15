//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-int.h"
#include "llvm-context.h"

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
    if (info.Length() < 2 || !LLVMContextWrapper::isInstance(info[0]) || !info[1]->IsNumber() ||
            (info.Length() > 2 && !info[2]->IsNumber() && !info[2]->IsUndefined()) ||
            (info.Length() > 3 && !info[3]->IsBoolean()) ||
            info.Length() > 4) {
        return Nan::ThrowTypeError("get needs to be called with: context: LLVMContext, value: number, numBits = 32, signed= true");
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    int64_t number = Nan::To<int64_t >(info[1]).FromJust();
    uint32_t numBits = 32;
    bool isSigned = true;

    if (info.Length() > 2 && !info[2]->IsUndefined()) {
        numBits = Nan::To<uint32_t>(info[2]).FromJust();
    }

    if (info.Length() == 3) {
        isSigned = Nan::To<bool>(info[3]).FromJust();
    }

    auto* constant = llvm::ConstantInt::get(context, llvm::APInt { numBits, static_cast<uint64_t>(number), isSigned } );

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

VALUE_WRAPPER_OF_DEFINITION(ConstantInt, ConstantInt, constantInt)

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
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("value").ToLocalChecked(), ConstantIntWrapper::getValueApf);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}
