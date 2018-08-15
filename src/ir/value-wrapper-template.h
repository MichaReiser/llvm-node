#ifndef LLVM_NODE_VALUE_WRAPPER_TEMPLATE_H
#define LLVM_NODE_VALUE_WRAPPER_TEMPLATE_H

#include <nan.h>
#include "value.h"
#include "../util/from-value-mixin.h"

#define VALUE_WRAPPER_TEMPLATE_BEGIN(LLVM_NAME, NAME, NAME_LOWERCASE, BASE_CLASS, CONSTRUCTOR_ACCESS_SPECIFIER) \
class NAME##Wrapper: public BASE_CLASS##Wrapper, public FromValueMixin<NAME##Wrapper> {                         \
public:                                                                                                         \
    using FromValueMixin<NAME##Wrapper>::FromValue;                                                             \
    static NAN_MODULE_INIT(Init);                                                                               \
    static v8::Local<v8::Object> of(llvm::LLVM_NAME* value);                                                    \
    llvm::LLVM_NAME* get##NAME() {                                                                              \
        return static_cast<llvm::LLVM_NAME*>(ValueWrapper::getValue());                                         \
    }                                                                                                           \
                                                                                                                \
CONSTRUCTOR_ACCESS_SPECIFIER:                                                                                   \
    explicit NAME##Wrapper(llvm::LLVM_NAME* value) : BASE_CLASS##Wrapper { value } {}                           \
    static Nan::Persistent<v8::FunctionTemplate>& NAME_LOWERCASE##Template();                                   \
                                                                                                                \
private:                                                                                                        \

#define VALUE_WRAPPER_TEMPLATE_END };

#define VALUE_WRAPPER_OF_DEFINITION(LLVM_NAME, NAME, NAME_LOWERCASE)                                            \
v8::Local<v8::Object> NAME##Wrapper::of(llvm::LLVM_NAME* value) {                                               \
    auto constructorFunction = Nan::GetFunction(Nan::New(NAME_LOWERCASE##Template())).ToLocalChecked();         \
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External>(value) };                                           \
    auto instance = Nan::NewInstance(constructorFunction, 1, args).ToLocalChecked();                            \
    Nan::EscapableHandleScope escapeScope {};                                                                   \
    return escapeScope.Escape(instance);                                                                        \
}

#endif
