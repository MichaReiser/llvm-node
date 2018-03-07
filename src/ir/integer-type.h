#ifndef LLVM_NODE_INTEGER_TYPE_H
#define LLVM_NODE_INTEGER_TYPE_H

#include <nan.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include "llvm-context.h"
#include "../util/from-value-mixin.h"
#include "type.h"

class IntegerTypeWrapper: public TypeWrapper, public FromValueMixin<IntegerTypeWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::IntegerType *type);
    using FromValueMixin<IntegerTypeWrapper>::FromValue;

    llvm::IntegerType* getIntegerType();
    static bool isInstance(v8::Local<v8::Value> value);

protected:
    llvm::Type* type;
    explicit IntegerTypeWrapper(llvm::IntegerType* type) : TypeWrapper { type } {
    }

    static Nan::Persistent<v8::FunctionTemplate>& integerTypeTemplate();

private:
    // Static Methods
    static NAN_METHOD(New);
    static NAN_METHOD(get);
    static NAN_GETTER(getBitWidth);
};

#endif //LLVM_NODE_INTEGER_TYPE_H
