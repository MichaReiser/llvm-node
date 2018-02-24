//
// Created by Micha Reiser on 01.03.17.
//

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include "../util/string.h"
#include "value.h"
#include "basic-block.h"
#include "constant.h"
#include "type.h"
#include "phi-node.h"
#include "alloca-inst.h"
#include "call-inst.h"

Napi::Object ValueWrapper::Init(Napi::Env env, Napi::Object exports)
{
    auto value = Napi::GetFunction(Napi::New(env, valueTemplate()));
    (target).Set(Napi::String::New(env, "Value"), value);
}

Napi::FunctionReference &ValueWrapper::valueTemplate()
{
    static Napi::FunctionReference functionTemplate{};

    if (functionTemplate.IsEmpty())
    {
        Napi::FunctionReference localTemplate = Napi::Function::New(env, ValueWrapper::New);
        localTemplate->SetClassName(Napi::String::New(env, "Value"));


#if defined(LLVM_NODE_DEBUG)
        Napi::SetPrototypeMethod(localTemplate, "dump", ValueWrapper::dump);
#endif

        Napi::SetPrototypeMethod(localTemplate, "hasName", ValueWrapper::hasName);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "type"), ValueWrapper::getType);
        Napi::SetAccessor(localTemplate->InstanceTemplate(), Napi::String::New(env, "name"), ValueWrapper::getName,
                         ValueWrapper::setName);
        Napi::SetPrototypeMethod(localTemplate, "release", ValueWrapper::deleteValue);
        Napi::SetPrototypeMethod(localTemplate, "deleteValue", ValueWrapper::deleteValue);
        Napi::SetPrototypeMethod(localTemplate, "replaceAllUsesWith", ValueWrapper::replaceAllUsesWith);
        Napi::SetPrototypeMethod(localTemplate, "useEmpty", ValueWrapper::useEmpty);

        auto function = Napi::GetFunction(localTemplate);
        (function).Set(Napi::String::New(env, "MaxAlignmentExponent"), Napi::New(env, llvm::Value::MaxAlignmentExponent));
        (function).Set(Napi::String::New(env, "MaximumAlignment"), Napi::New(env, llvm::Value::MaximumAlignment));

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

Napi::Value ValueWrapper::New(const Napi::CallbackInfo& info)
{
    if (!info.IsConstructCall())
    {
        Napi::TypeError::New(env, "Class Constructor Value cannot be invoked without new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() != 1 || !info[0].IsExternal())
    {
        Napi::TypeError::New(env, "External Value Pointer required").ThrowAsJavaScriptException();
        return env.Null();
    }

    llvm::Value *value = static_cast<llvm::Value *>(*info[0].As<Napi::External>()->Value());
    auto *wrapper = new ValueWrapper{value};
    wrapper->Wrap(info.This());

    return info.This();
}

#if defined(LLVM_NODE_DEBUG)
Napi::Value ValueWrapper::dump(const Napi::CallbackInfo& info)
{
    ValueWrapper::FromValue(info.Holder())->value->dump();
}
#endif

Napi::Value ValueWrapper::getType(const Napi::CallbackInfo& info)
{
    auto *type = ValueWrapper::FromValue(info.Holder())->value->getType();

    return TypeWrapper::of(type);
}

Napi::Value ValueWrapper::hasName(const Napi::CallbackInfo& info)
{
    auto *wrapper = ValueWrapper::FromValue(info.Holder());

    return Napi::New(env, wrapper->value->hasName());
}

Napi::Value ValueWrapper::getName(const Napi::CallbackInfo& info)
{
    auto *wrapper = ValueWrapper::FromValue(info.Holder());

    Napi::String name = Napi::New(env, wrapper->value->getName().str());
    return name;
}

void ValueWrapper::setName(const Napi::CallbackInfo& info, const Napi::Value& value)
{
    if (!value.IsString())
    {
        Napi::TypeError::New(env, "String required").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto name = value.To<Napi::String>();
    ValueWrapper::FromValue(info.Holder())->value->setName(ToString(name));
}

Napi::Value ValueWrapper::deleteValue(const Napi::CallbackInfo& info)
{
    auto *wrapper = ValueWrapper::FromValue(info.Holder());
#if LLVM_VERSION_MAJOR == 4
    delete wrapper->getValue();
#else
    wrapper->getValue()->deleteValue();
#endif
}

Napi::Value ValueWrapper::replaceAllUsesWith(const Napi::CallbackInfo& info)
{
    if (info.Length() != 1 || !ValueWrapper::isInstance(info[0]))
    {
        Napi::TypeError::New(env, "replaceAllUsesWith needs to be called with: value: Value").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto *self = ValueWrapper::FromValue(info.Holder())->getValue();
    auto *value = ValueWrapper::FromValue(info[0])->getValue();

    self->replaceAllUsesWith(value);
}

Napi::Value ValueWrapper::useEmpty(const Napi::CallbackInfo& info)
{
    auto *value = ValueWrapper::FromValue(info.Holder())->getValue();
    return Napi::New(env, value->use_empty());
}

Napi::Object ValueWrapper::of(llvm::Value *value)
{
    Napi::Object result{};

    if (llvm::Constant::classof(value))
    {
        result = ConstantWrapper::of(static_cast<llvm::Constant *>(value));
    }
    else if (llvm::BasicBlock::classof(value))
    {
        result = BasicBlockWrapper::of(static_cast<llvm::BasicBlock *>(value));
    }
    else if (llvm::PHINode::classof(value))
    {
        result = PhiNodeWrapper::of(static_cast<llvm::PHINode *>(value));
    }
    else if (llvm::AllocaInst::classof(value))
    {
        result = AllocaInstWrapper::of(static_cast<llvm::AllocaInst *>(value));
    }
    else if (llvm::CallInst::classof(value))
    {
        result = CallInstWrapper::of(static_cast<llvm::CallInst *>(value));
    }
    else
    {
        auto constructorFunction = Napi::GetFunction(Napi::New(env, valueTemplate()));
        Napi::Value args[1] = {Napi::External::New(env, value)};
        result = Napi::NewInstance(constructorFunction, 1, args);
    }

    Napi::EscapableHandleScope escapeScope{};
    return escapeScope.Escape(result);
}

bool ValueWrapper::isInstance(Napi::Value value) {
    Napi::Env env = value.Env();
    return Napi::New(env, valueTemplate())->HasInstance(value);
}

llvm::Value *ValueWrapper::getValue()
{
    return value;
}
