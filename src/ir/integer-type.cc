
#include "integer-type.h"

NAN_MODULE_INIT(IntegerTypeWrapper::Init) {
    auto arrayType = Nan::GetFunction(Nan::New(integerTypeTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("IntegerType").ToLocalChecked(), arrayType);
}

v8::Local<v8::Object> IntegerTypeWrapper::of(llvm::IntegerType* integerType) {
    auto constructorFunction = Nan::GetFunction(Nan::New(integerTypeTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External> (integerType) };

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

bool IntegerTypeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(integerTypeTemplate())->HasInstance(value);
}

llvm::IntegerType* IntegerTypeWrapper::getIntegerType() {
    return static_cast<llvm::IntegerType*>(getType());
}

NAN_METHOD(IntegerTypeWrapper::getBitWidth) {
    auto* integerType = IntegerTypeWrapper::FromValue(info.Holder())->getIntegerType();
    unsigned bitWidth = integerType->getBitWidth();

    info.GetReturnValue().Set(Nan::New(bitWidth));
}

v8::Persistent<v8::FunctionTemplate>& IntegerTypeWrapper::integerTypeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        auto integerTypeWrapperTemplate = Nan::New<v8::FunctionTemplate>(IntegerTypeWrapper::New);
        integerTypeWrapperTemplate->SetClassName(Nan::New("ArrayType").ToLocalChecked());
        integerTypeWrapperTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        integerTypeWrapperTemplate->Inherit(Nan::New(typeTemplate()));

        Nan::SetMethod(integerTypeWrapperTemplate, "getBitWidth", IntegerTypeWrapper::getBitWidth);

        tmpl.Reset(integerTypeWrapperTemplate);
    }

    return tmpl;
}
