#include <llvm/Support/TargetSelect.h>
#include "support.h"

NAN_METHOD(InitializeAllTargetInfos) {
    llvm::InitializeAllTargetInfos();
}

NAN_METHOD(InitializeAllTargets) {
    llvm::InitializeAllTargets();
}

NAN_METHOD(InitializeAllTargetMCs) {
    llvm::InitializeAllTargetMCs();
}

NAN_METHOD(InitializeAllAsmParsers) {
    llvm::InitializeAllAsmParsers();
}

NAN_METHOD(InitializeAllAsmPrinters) {
    llvm::InitializeAllAsmPrinters();
}

NAN_MODULE_INIT(InitSupport) {
    Nan::Set(target, 
        Nan::New<v8::String>("InitializeAllTargetInfos").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(InitializeAllTargetInfos)).ToLocalChecked()
    );

    Nan::Set(target, 
        Nan::New<v8::String>("InitializeAllTargets").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(InitializeAllTargets)).ToLocalChecked()
    );

     Nan::Set(target, 
        Nan::New<v8::String>("InitializeAllTargetMCs").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(InitializeAllTargetMCs)).ToLocalChecked()
    );

     Nan::Set(target, 
        Nan::New<v8::String>("InitializeAllAsmParsers").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(InitializeAllAsmParsers)).ToLocalChecked()
    );

     Nan::Set(target, 
        Nan::New<v8::String>("InitializeAllAsmPrinters").ToLocalChecked(),
        Nan::GetFunction(Nan::New<v8::FunctionTemplate>(InitializeAllAsmPrinters)).ToLocalChecked()
    );

    TargetRegistryWrapper::Init(target);
    TargetWrapper::Init(target);
}