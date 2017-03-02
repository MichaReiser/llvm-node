//
// Created by Micha Reiser on 02.03.17.
//

#ifndef LLVM_NODE_VERIFIER_H
#define LLVM_NODE_VERIFIER_H

#include <nan.h>
#include <llvm/IR/Verifier.h>

NAN_MODULE_INIT(InitVerifier);

NAN_METHOD(verifyModule);
NAN_METHOD(verifyFunction);

#endif //LLVM_NODE_VERIFIER_H
