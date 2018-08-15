//
// Created by Micha Reiser on 01.03.17.
//

#include "phi-node.h"
#include "basic-block.h"

VALUE_WRAPPER_OF_DEFINITION(PHINode, PhiNode, phiNode)

NAN_MODULE_INIT(PhiNodeWrapper::Init) {
    auto functionTemplate = Nan::New<v8::FunctionTemplate>(PhiNodeWrapper::New);

    functionTemplate->SetClassName(Nan::New("PHINode").ToLocalChecked());
    functionTemplate->InstanceTemplate()->SetInternalFieldCount(1);
    functionTemplate->Inherit(Nan::New(valueTemplate()));

    Nan::SetPrototypeMethod(functionTemplate, "addIncoming", PhiNodeWrapper::addIncoming);

    phiNodeTemplate().Reset(functionTemplate);
}

NAN_METHOD(PhiNodeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Class Constructor PhiNode cannot be invoked without new");
    }

    if (info.Length() != 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("PhiNode constructor needs be called with external pointer to PHINode");
    }

    auto* phiNode = static_cast<llvm::PHINode*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new PhiNodeWrapper { phiNode };
    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(PhiNodeWrapper::addIncoming) {
    if (info.Length() != 2 || !ValueWrapper::isInstance(info[0]) || !BasicBlockWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("addIncome needs to be called with: value: Value, basicBlock: BasicBlock");
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* basicBlock = BasicBlockWrapper::FromValue(info[1])->getBasicBlock();

    PhiNodeWrapper::FromValue(info.Holder())->getPhiNode()->addIncoming(value, basicBlock);
}

Nan::Persistent<v8::FunctionTemplate>& PhiNodeWrapper::phiNodeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};
    return functionTemplate;
}
