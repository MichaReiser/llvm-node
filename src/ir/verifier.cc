//
// Created by Micha Reiser on 02.03.17.
//

#include <llvm/Support/raw_ostream.h>
#include "verifier.h"
#include "function.h"
#include "module.h"

NAN_MODULE_INIT(InitVerifier) {
    Nan::SetMethod(target, "verifyFunction", verifyFunction);
    Nan::SetMethod(target, "verifyModule", verifyModule);
}

NAN_METHOD(verifyFunction) {
    if (info.Length() != 1 || !FunctionWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("verifyFunction needs to be called with: fun: Function");
    }

    auto* function = FunctionWrapper::FromValue(info[0])->getFunction();
    static std::string msgPrefix { "Function verification failed: " };
    llvm::raw_string_ostream output { msgPrefix };

    if (llvm::verifyFunction(*function, &output)) {
        return Nan::ThrowError(output.str().c_str());
    }

    info.GetReturnValue().Set(Nan::New(false));
}

NAN_METHOD(verifyModule) {
    if (info.Length() != 1 || !ModuleWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("verifyModule needs to be called with: mod: Module");
    }

    auto* module = ModuleWrapper::FromValue(info[0])->getModule();
    static std::string msgPrefix { "Module verification failed: " };
    llvm::raw_string_ostream output { msgPrefix };

    if (llvm::verifyModule(*module, &output)) {
        return Nan::ThrowError(output.str().c_str());
    }

    info.GetReturnValue().Set(Nan::New(false));
}