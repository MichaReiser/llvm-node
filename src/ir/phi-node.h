//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_PHINODEWRAPPER_H
#define LLVM_NODE_PHINODEWRAPPER_H

#include<napi.h>
#include<llvm/IR/Instructions.h>
#include "value.h"

class PhiNodeWrapper: public ValueWrapper, public FromValueMixin<PhiNodeWrapper> {
public:
    llvm::PHINode* getPhiNode();

    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object of(llvm::PHINode* phiNode);
    using FromValueMixin<PhiNodeWrapper>::FromValue;

private:
    explicit PhiNodeWrapper(llvm::PHINode* phiNode)
            : ValueWrapper { phiNode }
    {}

    static Napi::Value New(const Napi::CallbackInfo& info);
    static Napi::Value addIncoming(const Napi::CallbackInfo& info);

    static inline Napi::FunctionReference& phiNodeTemplate() {
        static Napi::FunctionReference functionTemplate {};
        return functionTemplate;
    }
};


#endif //LLVM_NODE_PHINODEWRAPPER_H
