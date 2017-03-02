//
// Created by Micha Reiser on 28.02.17.
//

#ifndef LLVM_NODE_TYPE_H
#define LLVM_NODE_TYPE_H

#include <nan.h>
#include <llvm/IR/Type.h>
#include "llvm-context.h"
#include "../util/from-value-mixin.h"

class TypeWrapper: public Nan::ObjectWrap, public FromValueMixin<TypeWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::Type *type);
    using FromValueMixin::FromValue;

    llvm::Type* getType();

    static bool isInstance(v8::Local<v8::Value> value);

protected:
    llvm::Type* type;
    TypeWrapper(llvm::Type* type) : Nan::ObjectWrap {}, type { type } {
        assert(type && "No type pointer passed");
    }

    static Nan::Persistent<v8::Object>& getObjectWithStaticMethods();
    static Nan::Persistent<v8::FunctionTemplate>& typeTemplate();

    // Static Methods
    static NAN_METHOD(New);

private:
    static NAN_METHOD(getDoubleTy);
    static NAN_METHOD(getVoidTy);

    // Instance Methods
    static NAN_METHOD(isDoubleTy);
    static NAN_GETTER(getTypeID);
};

#endif //LLVM_NODE_TYPE_H
