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
    typedef llvm::Type* (getTypeFn)(llvm::LLVMContext&);
    template<getTypeFn method>
    static NAN_METHOD(getType);

    typedef llvm::IntegerType* (getIntTypeFn)(llvm::LLVMContext&);
    template<getIntTypeFn method>
    static NAN_METHOD(getIntType);

    typedef bool (llvm::Type::*isTy)() const;
    template<isTy method>
    static NAN_METHOD(isOfType);

    // Instance Methods
    static NAN_GETTER(getTypeID);
};

#endif //LLVM_NODE_TYPE_H
