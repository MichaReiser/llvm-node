
#include "execution-engine.h"
#include "../ir/module.h"

Nan::Persistent<v8::ObjectTemplate> ExecutionEngineWrapper::executionEngineTemplate {};

NAN_MODULE_INIT(ExecutionEngineWrapper::Init) {
    auto objectTemplate = Nan::New<v8::ObjectTemplate>();
    objectTemplate->SetInternalFieldCount(1);

    Nan::SetMethod(objectTemplate, "addModule", ExecutionEngineWrapper::addModule);

    executionEngineTemplate.Reset(objectTemplate);
}

NAN_METHOD(ExecutionEngineWrapper::addModule) {
    if (info.Length() != 1 || !ModuleWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("addModule needs to be called with: mod: Module");
    }

    auto* module = ModuleWrapper::FromValue(info[0])->getModule();

    auto* executionEngine = ExecutionEngineWrapper::FromValue(info.Holder())->executionEngine;
    executionEngine->addModule(std::make_unique<llvm::Module*>(module));
}

v8::Local<v8::Object> ExecutionEngineWrapper::of(const llvm::ExecutionEngine *executionEnginePtr) {
    Nan::EscapableHandleScope escapeScope {};
    v8::Local<v8::ObjectTemplate> tpl = Nan::New(executionEngineTemplate);

    auto object = Nan::NewInstance(tpl).ToLocalChecked();
    auto* wrapper = new ExecutionEngineWrapper { executionEnginePtr };
    wrapper->Wrap(object);

    return escapeScope.Escape(object);
}
