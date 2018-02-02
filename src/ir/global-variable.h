//
// Created by Micha Reiser on 24.03.17.
//

#ifndef LLVM_NODE_GLOBAL_VARIABLE_H
#define LLVM_NODE_GLOBAL_VARIABLE_H

#include <nan.h>
#include <llvm/IR/GlobalVariable.h>
#include "constant.h"
#include "../util/from-value-mixin.h"

class GlobalVariableWrapper: public ConstantWrapper, public FromValueMixin<GlobalVariableWrapper> {
public:

    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::GlobalVariable* variable);
    using FromValueMixin<GlobalVariableWrapper>::FromValue;
    llvm::GlobalVariable* getGlobalVariable();


private:
    explicit GlobalVariableWrapper(llvm::GlobalVariable* variable) : ConstantWrapper { variable } {}

    static NAN_METHOD(New);
    static NAN_METHOD(NewFromExternal);
    static NAN_METHOD(NewFromArguments);

    static NAN_METHOD(hasGlobalUnnamedAddr);
    static NAN_METHOD(setUnnamedAddr);

    static NAN_GETTER(getConstant);
    static NAN_SETTER(setConstant);

    static NAN_GETTER(getInitializer);
    static NAN_SETTER(setInitializer);
    static Nan::Persistent<v8::FunctionTemplate>& globalVariableTemplate();

};

#endif //LLVM_NODE_GLOBAL_VARIABLE_H
