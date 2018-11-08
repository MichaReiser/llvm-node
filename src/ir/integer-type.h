
#ifndef LLVM_NODE_INTEGER_TYPE_H
#define LLVM_NODE_INTEGER_TYPE_H\


#include <nan.h>
#include <llvm/IR/DerivedTypes.h>
#include "type.h"

class IntegerTypeWrapper : public TypeWrapper, public FromValueMixin<IntegerTypeWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::IntegerType* type);
    static bool isInstance(v8::Local<v8::Value> value);
    using FromValueMixin<IntegerTypeWrapper>::FromValue;
    llvm::IntegerType* getIntegerType();
private:
    explicit IntegerTypeWrapper(llvm::IntegerType* type) : TypeWrapper { type } {
    }

    static NAN_METHOD(New);
    static NAN_METHOD(getBitWidth);

    static v8::Persistent<v8::FunctionTemplate>& integerTypeTemplate();
};

#endif //LLVM_NODE_INTEGER_TYPE_H
