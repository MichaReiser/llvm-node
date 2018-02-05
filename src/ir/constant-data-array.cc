//
// Created by Micha Reiser on 24.03.17.
//

#include "constant-data-array.h"
#include "llvm-context.h"
#include "../util/array.h"
#include "../util/string.h"

NAN_MODULE_INIT(ConstantDataArrayWrapper::Init) {
    auto constantDataArray = Nan::GetFunction(Nan::New(constantDataArrayTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("ConstantDataArray").ToLocalChecked(), constantDataArray);
}

v8::Local<v8::Object> ConstantDataArrayWrapper::of(llvm::ConstantDataArray* constantDataArray) {
    Nan::EscapableHandleScope escapeScope {};
    auto constuctorFunction = Nan::GetFunction(Nan::New(constantDataArrayTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(constantDataArray) };
    auto instance = Nan::NewInstance(constuctorFunction, 1, args).ToLocalChecked();

    return escapeScope.Escape(instance);
}

llvm::ConstantDataArray* ConstantDataArrayWrapper::getConstantDataArray() {
    return static_cast<llvm::ConstantDataArray*>(getValue());
}

Nan::Persistent<v8::FunctionTemplate>& ConstantDataArrayWrapper::constantDataArrayTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(ConstantDataArrayWrapper::New);
        localTemplate->SetClassName(Nan::New("ConstantDataArray").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(constantTemplate()));

        Nan::SetMethod(localTemplate, "get", ConstantDataArrayWrapper::get);
        Nan::SetMethod(localTemplate, "getString", ConstantDataArrayWrapper::getString);


        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

NAN_METHOD(ConstantDataArrayWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("The constructor of ConstantDataArray needs to be called with new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("The constructor of ConstantDataArray needs to be called width: value: External");
    }

    auto* value = static_cast<llvm::ConstantDataArray*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ConstantDataArrayWrapper { value };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ConstantDataArrayWrapper::getString) {
    if (info.Length() != 2 || !LLVMContextWrapper::isInstance(info[0]) || !info[1]->IsString()) {
        return Nan::ThrowTypeError("getString needs to be called with: context: LLVMContext, value: string");
    }

    auto& llvmContext = LLVMContextWrapper::FromValue(info[0])->getContext();
    auto initializer = ToString(info[1]);

    auto* constantArray = llvm::ConstantDataArray::getString(llvmContext, initializer);
    info.GetReturnValue().Set(ConstantWrapper::of(constantArray));
}

NAN_METHOD(ConstantDataArrayWrapper::get) {
    if (info.Length() != 2 || !LLVMContextWrapper::isInstance(info[0]) ||
            !(info[1]->IsUint32Array() || info[1]->IsFloat32Array() || info[1]->IsFloat64Array())) {
        return Nan::ThrowTypeError("get needs to be called with: context: LLVMContext, arrayRef: UInt32Array | Float32Array | Float64Array");
    }

    auto& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    llvm::Constant* constant;

    if (info[1]->IsUint32Array()) {
        auto ints = toVector<uint32_t , v8::Uint32Array>(info[1]);
        constant = llvm::ConstantDataArray::get(context, ints);
    } else if (info[1]->IsFloat32Array()) {
        auto floatArray = info[1].As<v8::Float32Array>();
        std::vector<float> floats (floatArray->Length());

        for (uint32_t i = 0; i < floatArray->Length(); ++i) {
            auto nativeValue = Nan::Get(floatArray, i).ToLocalChecked();
            floats[i] = static_cast<float>(Nan::To<double>(nativeValue).FromJust());
        }
        constant = llvm::ConstantDataArray::get(context, floats);
    } else {
        auto doubles = toVector<double, v8::Float64Array>(info[1]);
        constant = llvm::ConstantDataArray::get(context, doubles);
    }

    info.GetReturnValue().Set(ConstantWrapper::of(constant));
}