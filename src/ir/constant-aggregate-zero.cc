#include "constant-aggregate-zero.h"
#include "type.h"

NAN_MODULE_INIT(ConstantAggregateZeroWrapper::Init) {
    auto constantAggregateZero = Nan::GetFunction(Nan::New(constantAggregateZeroTemplate())).ToLocalChecked();

    Nan::Set(target, Nan::New("ConstantAggregateZero").ToLocalChecked(), constantAggregateZero);
}

v8::Local<v8::Object> ConstantAggregateZeroWrapper::of(llvm::ConstantAggregateZero* constantAggregateZero) {
    auto constructor = Nan::GetFunction(Nan::New(constantAggregateZeroTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External> (constantAggregateZero) };

    auto instance = Nan::NewInstance(constructor, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::ConstantAggregateZero* ConstantAggregateZeroWrapper::getConstantAggregateZero() {
    return static_cast<llvm::ConstantAggregateZero*>(getValue());
}

Nan::Persistent<v8::FunctionTemplate>& ConstantAggregateZeroWrapper::constantAggregateZeroTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(ConstantAggregateZeroWrapper::New);
        localTemplate->SetClassName(Nan::New("ConstantAggregateZero").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(constantTemplate()));

        Nan::SetMethod(localTemplate, "get", ConstantAggregateZeroWrapper::get);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

NAN_METHOD(ConstantAggregateZeroWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("ConstantAggregateZero constructor needs to be called with new");
    }

    if (!info[0]->IsExternal()) {
        return Nan::ThrowTypeError("ConstantAggregateZero constructor needs to be called with: constantAggregateZero: External");
    }

    auto* constantAggregateZero = static_cast<llvm::ConstantAggregateZero*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ConstantAggregateZeroWrapper { constantAggregateZero };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}


NAN_METHOD(ConstantAggregateZeroWrapper::get) {
    if (info.Length() != 2 || !TypeWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("get needs to be called with: type: Type");
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto constantAggregateZero = llvm::ConstantAggregateZero::get(type);

    info.GetReturnValue().Set(ConstantWrapper::of(constantAggregateZero));
}
