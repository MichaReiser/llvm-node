//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-fp.h"
#include "llvm-context.h"
#include "type.h"

NAN_MODULE_INIT(ConstantFPWrapper::Init) {
        auto constantFp = Nan::GetFunction(Nan::New(constantFpTemplate())).ToLocalChecked();
        Nan::Set(target, Nan::New("ConstantFP").ToLocalChecked(), constantFp);
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
        double number = Nan::To<double>(info[1]).FromJust();

        auto* constant = llvm::ConstantFP::get(context, llvm::APFloat { number } );

        info.GetReturnValue().Set(ConstantFPWrapper::of(constant));
}

NAN_METHOD(ConstantFPWrapper::getNaN) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("getNaN needs to be called with: type: Type");
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    llvm::Constant* nan = llvm::ConstantFP::getNaN(type);

    info.GetReturnValue().Set(ConstantWrapper::of(nan));
}

NAN_METHOD(ConstantFPWrapper::getInfinity) {
    if (
        (info.Length() > 2 || info.Length() == 0)
        || !TypeWrapper::isInstance(info[0])
        || (info.Length() == 2 && !info[1]->IsBoolean())
    ) {
        return Nan::ThrowTypeError("getInfinity needs to be called with: type: Type, negative?: boolean = false");
    }

    bool negative = false;

    if (info.Length() == 2) {
        negative = Nan::To<bool>(info[1]).FromJust();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    llvm::Constant* infinity = llvm::ConstantFP::getInfinity(type, negative);

    info.GetReturnValue().Set(ConstantWrapper::of(infinity));
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

        Nan::SetMethod(localTemplate, "get", ConstantFPWrapper::get);
        Nan::SetMethod(localTemplate, "getNaN", ConstantFPWrapper::getNaN);
        Nan::SetMethod(localTemplate, "getInfinity", ConstantFPWrapper::getInfinity);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("value").ToLocalChecked(), ConstantFPWrapper::getValueAPF);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}
