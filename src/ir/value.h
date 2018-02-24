//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_VALUE_H
#define LLVM_NODE_VALUE_H

#include <nan.h>
#include <llvm/IR/Value.h>
#include "../util/from-value-mixin.h"

class ValueWrapper: public Nan::ObjectWrap, public FromValueMixin<ValueWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::Value* value);
    static bool isInstance(v8::Local<v8::Value> value);
    llvm::Value *getValue();

protected:
    static Nan::Persistent<v8::FunctionTemplate>& valueTemplate();

    explicit ValueWrapper(llvm::Value* value)
            : Nan::ObjectWrap {}, value { value } {
        assert(value && "value pointer is missing");
    }


private:
    llvm::Value* value;

    static NAN_METHOD(New);
    static NAN_METHOD(dump);
    static NAN_GETTER(getType);
    static NAN_METHOD(hasName);
    static NAN_GETTER(getName);
    static NAN_SETTER(setName);
    static NAN_METHOD(deleteValue);
    static NAN_METHOD(replaceAllUsesWith);
    static NAN_METHOD(useEmpty);
};


#endif //LLVM_NODE_VALUE_H
