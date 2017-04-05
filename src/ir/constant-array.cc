//
// Created by Micha Reiser on 04.04.17.
//

#include "constant-array.h"
#include "array-type.h"

NAN_MODULE_INIT(ConstantArrayWrapper::Init) {
    auto constantArray = Nan::GetFunction(Nan::New(constantArrayTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("ConstantArray").ToLocalChecked(), constantArray);
}

v8::Local<v8::Object> ConstantArrayWrapper::of(llvm::ConstantArray* constantArray) {
    auto constructorFunction = Nan::GetFunction(Nan::New(constantArrayTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(constantArray)};

    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

Nan::Persistent<v8::FunctionTemplate>& ConstantArrayWrapper::constantArrayTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(ConstantArrayWrapper::New);
        localTemplate->SetClassName(Nan::New("ConstantArray").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(constantTemplate()));

        Nan::SetMethod(localTemplate, "get", ConstantArrayWrapper::get);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

NAN_METHOD(ConstantArrayWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("The constructor of ConstantArray needs to be called with new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("The constructor of ConstantArray needs to be called width: value: External");
    }

    auto* value = static_cast<llvm::ConstantArray*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ConstantArrayWrapper { value };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ConstantArrayWrapper::get) {
    if (info.Length() != 2 || !ArrayTypeWrapper::isInstance(info[0]) || !info[1]->IsArray()) {
        return Nan::ThrowTypeError("ConstantArray.get needs to be called with: type: ArrayType, values: constants");
    }

    auto* arrayType = ArrayTypeWrapper::FromValue(info[0])->getArrayType();
    auto jsArray = info[1].As<v8::Array>();
    std::vector<llvm::Constant*> constants { jsArray->Length() };

    for (uint32_t i = 0; i < jsArray->Length(); ++i) {
        auto value = Nan::Get(jsArray, i).ToLocalChecked();
        if (!ConstantWrapper::isInstance(value)) {
            return Nan::ThrowTypeError("get expected array of Constant values");
        }

        constants[i] = ConstantWrapper::FromValue(value)->getConstant();
    }

    auto* result = llvm::ConstantArray::get(arrayType, constants);
    info.GetReturnValue().Set(ConstantWrapper::of(result));
}
