//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_ARGUMENT_H
#define LLVM_NODE_ARGUMENT_H

#include <nan.h>
#include <llvm/IR/Argument.h>
#include "value.h"
#include "../util/from-value-mixin.h"

class ArgumentWrapper: public ValueWrapper, public FromValueMixin<ArgumentWrapper> {
public:
    llvm::Argument* getArgument();

    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::Argument* argument);
    using FromValueMixin<ArgumentWrapper>::FromValue;

private:
    explicit ArgumentWrapper(llvm::Argument *argument)
        : ValueWrapper { argument }
    {}

    static NAN_METHOD(New);
    static NAN_GETTER(getArgNo);
    static NAN_GETTER(getParent);
    static NAN_METHOD(addAttr);
    static NAN_METHOD(hasAttribute);
    static NAN_METHOD(addDereferenceableAttr);

    static inline Nan::Persistent<v8::FunctionTemplate>& argumentTemplate() {
        static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};
        return functionTemplate;
    }
};


#endif //LLVM_NODE_ARGUMENT_H
