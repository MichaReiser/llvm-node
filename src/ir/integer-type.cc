#include "integer-type.h"

NAN_MODULE_INIT(IntegerTypeWrapper::Init) {
    auto integerType = Nan::GetFunction(Nan::New(integerTypeTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("IntegerType").ToLocalChecked(), integerType);
}

NAN_METHOD(IntegerTypeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected pointer type pointer");
    }

    auto* type = static_cast<llvm::IntegerType*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new IntegerTypeWrapper { type };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(IntegerTypeWrapper::get) {
    if (info.Length() != 2 || !LLVMContextWrapper::isInstance(info[0]) || !info[1]->IsUint32()) {
        return Nan::ThrowTypeError("IntegerTypeWrapper.get needs to be called with: context: LLVMContext, bitWidth: uint32");
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    uint32_t bitWidth = Nan::To<uint32_t>(info[1]).FromJust();

    auto* integerType = llvm::IntegerType::get(context, bitWidth);

    info.GetReturnValue().Set(IntegerTypeWrapper::of(integerType));
}

NAN_GETTER(IntegerTypeWrapper::getBitWidth) {
    auto* integerType = IntegerTypeWrapper::FromValue(info.Holder())->getIntegerType();
    uint32_t bitWidth = integerType->getBitWidth();

    info.GetReturnValue().Set(Nan::New(bitWidth));
}

v8::Local<v8::Object> IntegerTypeWrapper::of(llvm::IntegerType *type) {
    Nan::EscapableHandleScope escapeScope {};

    v8::Local<v8::FunctionTemplate> functionTemplate = Nan::New(integerTypeTemplate());
    auto constructorFunction = Nan::GetFunction(functionTemplate).ToLocalChecked();
    v8::Local<v8::Value> argv[1] = { Nan::New<v8::External>(type) };
    v8::Local<v8::Object> object = Nan::NewInstance(constructorFunction, 1, argv).ToLocalChecked();

    return escapeScope.Escape(object);
}

Nan::Persistent<v8::FunctionTemplate>& IntegerTypeWrapper::integerTypeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> persistentTemplate {};

    if (persistentTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> integerTypeTemplate = Nan::New<v8::FunctionTemplate>(IntegerTypeWrapper::New);

        Nan::SetMethod(integerTypeTemplate, "get", IntegerTypeWrapper::get);
        integerTypeTemplate->SetClassName(Nan::New("IntegerType").ToLocalChecked());
        integerTypeTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        integerTypeTemplate->Inherit(Nan::New(typeTemplate()));

        Nan::SetAccessor(integerTypeTemplate->InstanceTemplate(), Nan::New("bitWidth").ToLocalChecked(), IntegerTypeWrapper::getBitWidth);

        persistentTemplate.Reset(integerTypeTemplate);
    }

    return persistentTemplate;
}

bool IntegerTypeWrapper::isInstance(v8::Local<v8::Value> object) {
    return Nan::New(integerTypeTemplate())->HasInstance(object);
}

llvm::IntegerType *IntegerTypeWrapper::getIntegerType() {
    return static_cast<llvm::IntegerType*>(getType());
}
