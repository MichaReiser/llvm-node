//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-struct.h"
#include "struct-type.h"

NAN_MODULE_INIT(ConstantStructWrapper::Init) {
    auto constantStruct = Nan::GetFunction(Nan::New(constantStructTemplate())).ToLocalChecked();

    Nan::Set(target, Nan::New("ConstantStruct").ToLocalChecked(), constantStruct);
}

v8::Local<v8::Object> ConstantStructWrapper::of(llvm::ConstantStruct* constantStruct) {
    auto constructor = Nan::GetFunction(Nan::New(constantStructTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External> (constantStruct) };

    auto instance = Nan::NewInstance(constructor, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::ConstantStruct* ConstantStructWrapper::getConstantStruct() {
    return static_cast<llvm::ConstantStruct*>(getValue());
}

Nan::Persistent<v8::FunctionTemplate>& ConstantStructWrapper::constantStructTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(ConstantStructWrapper::New);
        localTemplate->SetClassName(Nan::New("ConstantStruct").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(constantTemplate()));

        Nan::SetMethod(localTemplate, "get", ConstantStructWrapper::get);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

NAN_METHOD(ConstantStructWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("ConstantStruct constructor needs to be called with new");
    }

    if (!info[0]->IsExternal()) {
        return Nan::ThrowTypeError("ConstantStruct constructor needs to be called with: constantStruct: External");
    }

    auto* constantStruct = static_cast<llvm::ConstantStruct*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ConstantStructWrapper { constantStruct };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}


NAN_METHOD(ConstantStructWrapper::get) {
    if (info.Length() != 2 || !StructTypeWrapper::isInstance(info[0]) || !info[1]->IsArray()) {
        return Nan::ThrowTypeError("get needs to be called with: structType: StructType, values: Constant[]");
    }

    auto* structType = StructTypeWrapper::FromValue(info[0])->getStructType();
    auto array = info[1].As<v8::Array>();
    std::vector<llvm::Constant*> constants { array->Length() };

    for (uint32_t i = 0; i < array->Length(); ++i) {
        auto value = Nan::Get(array, i).ToLocalChecked();
        if (!ConstantWrapper::isInstance(value)) {
            return Nan::ThrowTypeError("get expected array of Constant values");
        }

        constants[i] = ConstantWrapper::FromValue(value)->getConstant();
    }

    auto constantStruct = llvm::ConstantStruct::get(structType, constants);

    info.GetReturnValue().Set(ConstantWrapper::of(constantStruct));
}