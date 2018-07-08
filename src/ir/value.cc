//
// Created by Micha Reiser on 01.03.17.
//

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include "value.h"

Napi::FunctionReference ValueWrapper::constructor;

void ValueWrapper::Init(Napi::Env env, Napi::Object &exports){
    Napi::Function func = DefineClass(env, "Value", {
        InstanceMethod("hasName", &ValueWrapper::hasName),
        InstanceAccessor("type", &ValueWrapper::getType, nullptr),
        InstanceAccessor("name", &ValueWrapper::getName, &ValueWrapper::setName),
        InstanceMethod("release", &ValueWrapper::deleteValue),
        InstanceMethod("deleteValue", &ValueWrapper::deleteValue),
        InstanceMethod("replaceAllUsesWith", &ValueWrapper::replaceAllUsesWith),
        InstanceMethod("useEmpty", &ValueWrapper::useEmpty),
        StaticValue("MaxAlignmentExponent", Napi::Number::New(env, llvm::Value::MaxAlignmentExponent)),
        StaticValue("MaximumAlignment", Napi::Number::New(env, llvm::Value::MaximumAlignment))
#if defined(LLVM_NODE_DEBUG)
        , InstanceMethod("dump", &ValueWrapper::dump)
#endif
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Value", func);
}


Napi::Value ValueWrapper::of(Napi::Env env, llvm::Value *value)
{
    Napi::Value result{};

//    TODO Uncomment when time has come
//    if (llvm::Constant::classof(value))
//    {
//        result = ConstantWrapper::of(static_cast<llvm::Constant *>(value));
//    }
//    else if (llvm::BasicBlock::classof(value))
//    {
//        result = BasicBlockWrapper::of(static_cast<llvm::BasicBlock *>(value));
//    }
//    else if (llvm::PHINode::classof(value))
//    {
//        result = PhiNodeWrapper::of(static_cast<llvm::PHINode *>(value));
//    }
//    else if (llvm::AllocaInst::classof(value))
//    {
//        result = AllocaInstWrapper::of(static_cast<llvm::AllocaInst *>(value));
//    }
//    else if (llvm::CallInst::classof(value))
//    {
//        result = CallInstWrapper::of(static_cast<llvm::CallInst *>(value));
//    }
//    else
//    {
        result = constructor.New({ Napi::External<llvm::Value>::New(env, value) });
//    }

    return result;
}

bool ValueWrapper::isInstanceOfType(Napi::Value value)
{
    return value.As<Napi::Object>().InstanceOf(constructor.Value());
}

ValueWrapper::ValueWrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<ValueWrapper> { info } {
    auto env = info.Env();

    if (!info.IsConstructCall())
    {
        throw Napi::TypeError::New(env, "Class Constructor Value cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0].IsExternal())
    {
        throw Napi::TypeError::New(env, "External Value Pointer required");
    }

    auto external = info[0].As<Napi::External<llvm::Value>>();
    this->value = external.Data();
}

#if defined(LLVM_NODE_DEBUG)
void ValueWrapper::dump(const Napi::CallbackInfo &info) {
    value->dump();
}
#endif

Napi::Value ValueWrapper::getType(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    auto* type = value->getType();

//    TODO uncomment when type wrapper is migrated
//    return TypeWrapper::of(type);

    return env.Undefined();
}

Napi::Value ValueWrapper::hasName(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    return Napi::Boolean::New(env, value->hasName());
}

Napi::Value ValueWrapper::getName(const Napi::CallbackInfo &info) {
    auto env = info.Env();
    return Napi::String::New(env, value->getName());
}

void ValueWrapper::setName(const Napi::CallbackInfo &info, const Napi::Value &value) {
    auto env = info.Env();
    if (!value.IsString())
    {
        throw Napi::TypeError::New(env, "name needs to be a string");
    }

    std::string name = value.As<Napi::String>();
    this->value->setName(name);
}

void ValueWrapper::deleteValue(const Napi::CallbackInfo &info) {
#if LLVM_VERSION_MAJOR == 4
    delete value;
#else
    value->deleteValue();
#endif
}

void ValueWrapper::replaceAllUsesWith(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    if (info.Length() != 1 || !ValueWrapper::isInstanceOfType(info[0]))
    {
        throw Napi::TypeError::New(env, "replaceAllUsesWith needs to be called with: value: Value");
    }

    auto* self = this->value;
    auto* value = ValueWrapper::Unwrap(info[0].As<Napi::Object>())->getValue();

    self->replaceAllUsesWith(value);
}

Napi::Value ValueWrapper::useEmpty(const Napi::CallbackInfo &info) {
    auto env = info.Env();

    return Napi::Boolean::New(env, value->use_empty());
}

llvm::Value *ValueWrapper::getValue()
{
    return value;
}
