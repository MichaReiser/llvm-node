//
// Created by Micha Reiser on 01.03.17.
//

#include <llvm/IR/InstrTypes.h>
#include "function.h"
#include "llvm-context.h"
#include "basic-block.h"
#include "../util/string.h"

NAN_MODULE_INIT(BasicBlockWrapper::Init) {
    auto basicBlock = Nan::GetFunction(Nan::New(basicBlockTemplate())).ToLocalChecked();
    Nan::Set(target, Nan::New("BasicBlock").ToLocalChecked(), basicBlock);
}

v8::Local<v8::Object> BasicBlockWrapper::of(llvm::BasicBlock *basicBlock) {
    auto constructorFunction = Nan::GetFunction(Nan::New(basicBlockTemplate())).ToLocalChecked();
    v8::Local<v8::Value> argv[1] = { Nan::New<v8::External>(basicBlock) };
    auto instance = Nan::NewInstance(constructorFunction, 1, argv).ToLocalChecked();

    Nan::EscapableHandleScope escapeScope {};
    return escapeScope.Escape(instance);
}

NAN_METHOD(BasicBlockWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor BasicBlock cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("BasicBlock constructor requires: basicBlock: External");
    }

    auto* basicBlock = static_cast<llvm::BasicBlock*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new BasicBlockWrapper { basicBlock };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(BasicBlockWrapper::Create) {
    if (info.Length() < 1 || !LLVMContextWrapper::isInstance(info[0])
        || (info.Length() > 1 && !info[1]->IsString())
        || (info.Length() > 2 && !FunctionWrapper::isInstance(info[2]))
        || (info.Length() > 3 && !BasicBlockWrapper::isInstance(info[3]))
        || info.Length() > 4) {
        return Nan::ThrowTypeError("BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?");
    }

    llvm::LLVMContext& context = LLVMContextWrapper::FromValue(info[0])->getContext();
    std::string name {};
    llvm::Function* parent = nullptr;
    llvm::BasicBlock* insertBefore = nullptr;

    if (info.Length() > 1) {
        name = ToString(Nan::To<v8::String>(info[1]).ToLocalChecked());
    }

    if (info.Length() > 2) {
        parent = FunctionWrapper::FromValue(info[2])->getFunction();
    }

    if (info.Length() > 3) {
        insertBefore = BasicBlockWrapper::FromValue(info[3])->getBasicBlock();
    }

    auto* basicBlock = llvm::BasicBlock::Create(context, name, parent, insertBefore);
    info.GetReturnValue().Set(BasicBlockWrapper::of(basicBlock));
}

NAN_GETTER(BasicBlockWrapper::empty) {
    auto* basicBlock = BasicBlockWrapper::FromValue(info.Holder())->getBasicBlock();
    info.GetReturnValue().Set(Nan::New(basicBlock->empty()));
}

NAN_METHOD(BasicBlockWrapper::eraseFromParent) {
    auto* basicBlock = BasicBlockWrapper::FromValue(info.Holder())->getBasicBlock();
    basicBlock->eraseFromParent();
}

NAN_GETTER(BasicBlockWrapper::getParent) {
    auto* wrapper = BasicBlockWrapper::FromValue(info.Holder());
    auto* function = wrapper->getBasicBlock()->getParent();

    if (function) {
        auto functionObject = FunctionWrapper::of(function);
        return info.GetReturnValue().Set(functionObject);
    }
}

NAN_GETTER(BasicBlockWrapper::getContext) {
    auto* wrapper = BasicBlockWrapper::FromValue(info.Holder());
    auto& context = wrapper->getBasicBlock()->getContext();

    info.GetReturnValue().Set(LLVMContextWrapper::of(context));
}


NAN_METHOD(BasicBlockWrapper::getTerminator) {
    auto* basicBlock = BasicBlockWrapper::FromValue(info.Holder())->getBasicBlock();
    auto* terminator = basicBlock->getTerminator();

    if (terminator) {
        return info.GetReturnValue().Set(ValueWrapper::of(terminator));
    }

    info.GetReturnValue().Set(Nan::Undefined());
}

Nan::Persistent<v8::FunctionTemplate>& BasicBlockWrapper::basicBlockTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        auto localTemplate = Nan::New<v8::FunctionTemplate>(BasicBlockWrapper::New);
        localTemplate->SetClassName(Nan::New("BasicBlock").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(ValueWrapper::valueTemplate()));

        Nan::SetMethod(localTemplate, "create", BasicBlockWrapper::Create);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("empty").ToLocalChecked(), BasicBlockWrapper::empty);
        Nan::SetPrototypeMethod(localTemplate, "eraseFromParent", BasicBlockWrapper::eraseFromParent);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("parent").ToLocalChecked(), BasicBlockWrapper::getParent);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("context").ToLocalChecked(), BasicBlockWrapper::getContext);
        Nan::SetPrototypeMethod(localTemplate, "getTerminator", BasicBlockWrapper::getTerminator);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

bool BasicBlockWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(basicBlockTemplate())->HasInstance(value);
}
