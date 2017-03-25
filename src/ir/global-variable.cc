
#include <llvm/IR/GlobalValue.h>
#include "global-variable.h"
#include "module.h"
#include "type.h"
#include "../util/string.h"
#include "../util/from-value-mixin.h"


NAN_MODULE_INIT(GlobalVariableWrapper::Init) {
    auto globalVariable = Nan::GetFunction(Nan::New(globalVariableTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("GlobalVariable").ToLocalChecked(), globalVariable);
}

v8::Local<v8::Object> GlobalVariableWrapper::of(llvm::GlobalVariable* variable) {
    auto constructor = Nan::GetFunction(Nan::New(globalVariableTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External> (variable) };

    auto instance = Nan::NewInstance(constructor, 1, args).ToLocalChecked();
    Nan::EscapableHandleScope escapeHandleScope {};
    return escapeHandleScope.Escape(instance);
}

llvm::GlobalVariable* GlobalVariableWrapper::getGlobalVariable() {
    return static_cast<llvm::GlobalVariable*>(getValue());
}

NAN_METHOD(GlobalVariableWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("The GlobalVariable needs to be called with new");
    }

    if (info.Length() == 1 && info[0]->IsExternal()) {
        NewFromExternal(info);
    } else if (info.Length() >= 4 && ModuleWrapper::isInstance(info[0]) && TypeWrapper::isInstance(info[1]) && info[2]->IsBoolean() && info[3]->IsUint32()
               && (info.Length() < 5 || ConstantWrapper::isInstance(info[4]) || info[4]->IsUndefined())
               && (info.Length() < 6 || info[5]->IsString())) {
        NewFromArguments(info);
    } else {
        return Nan::ThrowTypeError("The GlobalVariable constructor needs to be called with: module: Module, type: Type, isConstant: boolean, linkageType: LinkageTypes, initializer?: Constant, name?: string");
    }
}


NAN_METHOD(GlobalVariableWrapper::NewFromExternal) {
    auto* globalVariable = static_cast<llvm::GlobalVariable*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new GlobalVariableWrapper { globalVariable };
    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(GlobalVariableWrapper::NewFromArguments) {
    auto* module = ModuleWrapper::FromValue(info[0])->getModule();
    auto* type = TypeWrapper::FromValue(info[1])->getType();
    auto constant = Nan::To<bool>(info[2]).FromJust();
    auto linkageType = static_cast<llvm::GlobalValue::LinkageTypes>(Nan::To<uint32_t>(info[3]).FromJust());
    llvm::Constant* initializer = nullptr;
    std::string name {};

    if (info.Length() > 4 && !info[4]->IsUndefined()) {
        initializer = ConstantWrapper::FromValue(info[4])->getConstant();
    }

    if (info.Length() > 5) {
        name = ToString(info[5]);
    }

    auto* globalVariable = new llvm::GlobalVariable { *module, type, constant, linkageType, initializer, name };

    auto* wrapper = new GlobalVariableWrapper { globalVariable };
    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(GlobalVariableWrapper::hasGlobalUnnamedAddr) {
    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    info.GetReturnValue().Set(Nan::New(globalVariable->hasGlobalUnnamedAddr()));
}

NAN_METHOD(GlobalVariableWrapper::setUnnamedAddr) {
    if (!info[0]->IsUint32()) {
        return Nan::ThrowTypeError("setUnnamedAddr needs to be called with: addr: GlobalValue.UnnamedAddr");
    }

    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    auto value = Nan::To<uint32_t>(info[0]).FromJust();
    globalVariable->setUnnamedAddr(static_cast<llvm::GlobalValue::UnnamedAddr>(value));
}

NAN_GETTER(GlobalVariableWrapper::getConstant) {
    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    info.GetReturnValue().Set(Nan::New(globalVariable->isConstant()));
}

NAN_SETTER(GlobalVariableWrapper::setConstant) {
    if (!value->IsBoolean()) {
        return Nan::ThrowTypeError("GlobalVariable.constant is of type boolean");
    }

    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    globalVariable->setConstant(Nan::To<bool>(value).FromJust());
}

NAN_GETTER(GlobalVariableWrapper::getInitializer) {
    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    auto* initializer = globalVariable->getInitializer();

    if (initializer) {
        info.GetReturnValue().Set(ConstantWrapper::of(initializer));
    } else {
        info.GetReturnValue().Set(Nan::Undefined());
    }
}

NAN_SETTER(GlobalVariableWrapper::setInitializer) {
    if (!value->IsUndefined() && !ConstantWrapper::isInstance(value)) {
        return Nan::ThrowTypeError("GlobalVariable.initializer is of type Constant");
    }

    auto* globalVariable = GlobalVariableWrapper::FromValue(info.Holder())->getGlobalVariable();
    llvm::Constant* initializer = nullptr;

    if (!value->IsUndefined()) {
        initializer = ConstantWrapper::FromValue(value)->getConstant();
    }

    globalVariable->setInitializer(initializer);
}

Nan::Persistent<v8::FunctionTemplate>& GlobalVariableWrapper::globalVariableTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(GlobalVariableWrapper::New);
        localTemplate->SetClassName(Nan::New("GlobalVariable").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(constantTemplate()));

        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("constant").ToLocalChecked(), GlobalVariableWrapper::getConstant, GlobalVariableWrapper::setConstant);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("initializer").ToLocalChecked(), GlobalVariableWrapper::getInitializer, GlobalVariableWrapper::setInitializer);

        Nan::SetPrototypeMethod(localTemplate, "hasGlobalUnnamedAddr", GlobalVariableWrapper::hasGlobalUnnamedAddr);
        Nan::SetPrototypeMethod(localTemplate, "setUnnamedAddr", GlobalVariableWrapper::setUnnamedAddr);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}