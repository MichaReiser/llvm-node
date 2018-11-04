
#include "execution-engine.h"
#include "../ir/data-layout.h"

Nan::Persistent<v8::ObjectTemplate> ExecutionEngineWrapper::executionEngineTemplate {};

NAN_MODULE_INIT(ExecutionEngineWrapper::Init) {
    auto objectTemplate = Nan::New<v8::ObjectTemplate>();
    objectTemplate->SetInternalFieldCount(1);

    executionEngineTemplate.Reset(objectTemplate);
}

v8::Local<v8::Object> ExecutionEngineWrapper::of(const llvm::ExecutionEngine *executionEnginePtr) {
    Nan::EscapableHandleScope escapeScope {};
    v8::Local<v8::ObjectTemplate> tpl = Nan::New(executionEngineTemplate);

    auto object = Nan::NewInstance(tpl).ToLocalChecked();
    auto* wrapper = new ExecutionEngineWrapper { executionEnginePtr };
    wrapper->Wrap(object);

    return escapeScope.Escape(object);
}
