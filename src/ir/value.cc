//
// Created by Micha Reiser on 01.03.17.
//

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include "value.h"
//#include "type.h"
//#include "basic-block.h"
//#include "constant.h"
//#include "type.h"
//#include "phi-node.h"
//#include "alloca-inst.h"
//#include "call-inst.h"

Napi::FunctionReference ValueWrapper::constructor;

Napi::Object ValueWrapper::Init(Napi::Env env, Napi::Object exports) {

    Napi::Function func = DefineClass(env, "Value", {
//            InstanceMethod("type", &ValueWrapper::getType),
            InstanceAccessor("name", &ValueWrapper::getName, &ValueWrapper::setName),
            InstanceMethod("hasName", &ValueWrapper::hasName),
            InstanceMethod("release", &ValueWrapper::deleteValue),
            InstanceMethod("deleteValue", &ValueWrapper::deleteValue),
            InstanceMethod("replaceAllUsesWith", &ValueWrapper::replaceAllUsesWith),
            InstanceMethod("useEmpty", &ValueWrapper::useEmpty),
#if defined(LLVM_NODE_DEBUG)
            InstanceMethod("dump", &ValueWrapper::dump),
#endif
            StaticValue("MaxAlignmentExponent", Napi::Number::New(env, llvm::Value::MaxAlignmentExponent)),
            StaticValue("MaximumAlignment", Napi::Number::New(env, llvm::Value::MaximumAlignment))
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Value", func);
    return exports;
}

ValueWrapper::ValueWrapper(const Napi::CallbackInfo &info) : Napi::ObjectWrap<ValueWrapper>{info} {
    Napi::Env env = info.Env();
    if (!info.IsConstructCall()) {
        throw Napi::TypeError::New(env, "Value constructor needs to be invoked with new");
    }

    if (info.Length() < 1) {
        throw Napi::TypeError::New(env, "Expected value to be called with external pointer to llvm:Value");
    }

    Napi::External<llvm::Value> external = info[0].As<Napi::External<llvm::Value>>();
    this->value = external.Data();
}


#if defined(LLVM_NODE_DEBUG)
Napi::Value ValueWrapper::dump(const Napi::CallbackInfo &info) {
    this->value->dump();
    return info.Env().Null();
}
#endif

//Napi::Object ValueWrapper::getType(const Napi::CallbackInfo &info) {
//    auto *type = this->value->getType();
//    return TypeWrapper::of(type);
//}

Napi::Value ValueWrapper::hasName(const Napi::CallbackInfo &info) {
    return Napi::Boolean::New(info.Env(), this->value->hasName());
}

Napi::Value ValueWrapper::getName(const Napi::CallbackInfo &info) {
    return Napi::String::New(info.Env(), this->value->getName());
}

void ValueWrapper::setName(const Napi::CallbackInfo &info, const Napi::Value &value) {
    Napi::Env env = info.Env();
    if (!value.IsString()) {
        throw Napi::TypeError::New(env, "name needs to be a string");
    }

    const std::string name = value.ToString();
    this->value->setName(name);
}

void ValueWrapper::deleteValue(const Napi::CallbackInfo &info) {
#if LLVM_VERSION_MAJOR == 4
    delete this->value;
#else
    this->value->deleteValue();
#endif
}

void ValueWrapper::replaceAllUsesWith(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();

    if (info.Length() != 1 || !ValueWrapper::isInstanceOfType(info[0])) {
        throw Napi::TypeError::New(env, "replaceAllUsesWith needs to be called with: value: Value");
    }

    auto *value = ValueWrapper::Unwrap(info[0].ToObject())->value;
    this->value->replaceAllUsesWith(value);
}

Napi::Value ValueWrapper::useEmpty(const Napi::CallbackInfo &info) {
    return Napi::Boolean::New(info.Env(), value->use_empty());
}

Napi::Object ValueWrapper::of(Napi::Env env, llvm::Value *value) {
//    if (llvm::Constant::classof(value)) {
//        return ConstantWrapper::of(static_cast<llvm::Constant *>(value));
//    } else if (llvm::BasicBlock::classof(value)) {
//        result = BasicBlockWrapper::of(static_cast<llvm::BasicBlock *>(value));
//    } else if (llvm::PHINode::classof(value)) {
//        result = PhiNodeWrapper::of(static_cast<llvm::PHINode *>(value));
//    } else if (llvm::AllocaInst::classof(value)) {
//        result = AllocaInstWrapper::of(static_cast<llvm::AllocaInst *>(value));
//    } else if (llvm::CallInst::classof(value)) {
//        result = CallInstWrapper::of(static_cast<llvm::CallInst *>(value));
//    } else {
    Napi::EscapableHandleScope scope { env };
    Napi::Object object = constructor.New({ Napi::External<llvm::Value>::New(env, value) });
    return scope.Escape(object).ToObject();
//    }
}

bool ValueWrapper::isInstanceOfType(const Napi::Value& value) {
    return value.ToObject().InstanceOf(constructor.Value());
}

llvm::Value *ValueWrapper::getValue() {
    return value;
}
