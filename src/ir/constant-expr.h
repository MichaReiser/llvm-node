#ifndef LLVM_NODE_CONSTANT_EXPR_H
#define LLVM_NODE_CONSTANT_EXPR_H

#include <nan.h>
#include <llvm/IR/Constants.h>
#include "constant.h"

class ConstantExprWrapper: public ConstantWrapper, public FromValueMixin<ConstantExprWrapper> {
public:
    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::ConstantExpr* constantExpr);
    using FromValueMixin<ConstantExprWrapper>::FromValue;
    llvm::ConstantExpr* getConstantExpr();

private:
    explicit ConstantExprWrapper(llvm::ConstantExpr* constantExpr) : ConstantWrapper { constantExpr }
    {}

    static Nan::Persistent<v8::FunctionTemplate>& constantExprTemplate();

    static NAN_METHOD(New);
    static NAN_METHOD(getOr);
    static NAN_METHOD(getPointerBitCastOrAddrSpaceCast);
    static NAN_METHOD(getPointerCast);
};

#endif //LLVM_NODE_CONSTANT_EXPR_H
