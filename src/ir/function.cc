//
// Created by Micha Reiser on 01.03.17.
//

#include "function.h"
#include "function-type.h"
#include "../util/string.h"
#include "module.h"
#include "argument.h"
#include "basic-block.h"

FunctionWrapper::FunctionWrapper(llvm::Function *function) : ConstantWrapper(function) {}

NAN_MODULE_INIT(FunctionWrapper::Init) {
    v8::Local<v8::Object> object = Nan::New<v8::Object>();

    Nan::SetMethod(object, "create", FunctionWrapper::Create);

    Nan::Set(target, Nan::New("Function").ToLocalChecked(), object);
}

NAN_METHOD(FunctionWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor Function cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("External Function Pointer required");
    }

    llvm::Function* function = static_cast<llvm::Function*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new FunctionWrapper { function };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

Nan::Persistent<v8::FunctionTemplate> &FunctionWrapper::functionTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> tmpl {};

    if (tmpl.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(FunctionWrapper::New);
        v8::Local<v8::FunctionTemplate> valueTemplate = Nan::New(ConstantWrapper::constantTemplate());

        localTemplate->SetClassName(Nan::New("Function").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(valueTemplate);

        Nan::SetPrototypeMethod(localTemplate, "getArguments", FunctionWrapper::getArguments);
        Nan::SetPrototypeMethod(localTemplate, "addBasicBlock", FunctionWrapper::addBasicBlock);

        tmpl.Reset(localTemplate);
    }

    return tmpl;
}

v8::Local<v8::Object> FunctionWrapper::of(llvm::Function *function) {
    v8::Local<v8::FunctionTemplate> localTemplate = Nan::New(functionTemplate());
    v8::Local<v8::Function> constructor = Nan::GetFunction(localTemplate).ToLocalChecked();

    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(function) };

    v8::Local<v8::Object> instance = Nan::NewInstance(constructor, 1, args ).ToLocalChecked();

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

NAN_METHOD(FunctionWrapper::Create) {
    if (info.Length() < 2 || !FunctionTypeWrapper::isInstance(info[0]) || !info[1]->IsUint32()
        || (info.Length() > 2 && !info[2]->IsString())
        || (info.Length() > 3 && !ModuleWrapper::isInstance(info[3]))) {
        return Nan::ThrowTypeError("Create needs to be called with: functionType: FunctionType, linkageTypes: uint32, name: string?, module?: Module");
    }

    auto* functionType = FunctionTypeWrapper::FromValue(info[0])->functionType();
    auto linkageType = static_cast<llvm::GlobalValue::LinkageTypes>(Nan::To<uint32_t>(info[1]).FromJust());

    std::string name = {};
    llvm::Module* module = nullptr;

    if (info.Length() > 2) {
        name = ToString(Nan::To<v8::String>(info[2]).ToLocalChecked());
    }

    if (info.Length() == 4) {
        module = ModuleWrapper::FromValue(info[3])->getModule();
    }

    auto* function = llvm::Function::Create(functionType, linkageType, name, module);
    auto wrapper = of(function);
    info.GetReturnValue().Set(wrapper);
}

NAN_METHOD(FunctionWrapper::addBasicBlock) {
    if (info.Length() != 1 || !BasicBlockWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("addBasicBlock needs to be called with: block: BasicBlock");
    }

    auto* block = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();
    auto* wrapper = FunctionWrapper::FromValue(info.Holder());
    wrapper->getFunction()->getBasicBlockList().push_back(block);
}

NAN_METHOD(FunctionWrapper::getArguments) {
    auto* wrapper = FunctionWrapper::FromValue(info.Holder());
    auto result = Nan::New<v8::Array>(wrapper->getFunction()->arg_size());

    uint32_t i = 0;
    for (auto &arg : wrapper->getFunction()->args()) {
        result->Set(i, ArgumentWrapper::of(&arg));
        ++i;
    }

    info.GetReturnValue().Set(result);
}

llvm::Function *FunctionWrapper::getFunction() {
    return static_cast<llvm::Function*>(getValue());
}

bool FunctionWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTemplate())->HasInstance(value);
}
