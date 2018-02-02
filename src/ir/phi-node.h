//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_PHINODEWRAPPER_H
#define LLVM_NODE_PHINODEWRAPPER_H

#include<nan.h>
#include<llvm/IR/Instructions.h>
#include "value.h"

class PhiNodeWrapper: public ValueWrapper, public FromValueMixin<PhiNodeWrapper> {
public:
    llvm::PHINode* getPhiNode();

    static NAN_MODULE_INIT(Init);
    static v8::Local<v8::Object> of(llvm::PHINode* phiNode);
    using FromValueMixin<PhiNodeWrapper>::FromValue;

private:
    explicit PhiNodeWrapper(llvm::PHINode* phiNode)
            : ValueWrapper { phiNode }
    {}

    static NAN_METHOD(New);
    static NAN_METHOD(addIncoming);

    static inline Nan::Persistent<v8::FunctionTemplate>& phiNodeTemplate() {
        static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};
        return functionTemplate;
    }
};


#endif //LLVM_NODE_PHINODEWRAPPER_H
