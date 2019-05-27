#include "di-builder.h"
#include "di-file.h"
#include "di-compile-unit.h"
#include "module.h"
#include "../util/string.h"

NAN_MODULE_INIT(DIBuilderWrapper::Init) {
    v8::Local<v8::FunctionTemplate> functionTemplate = Nan::New<v8::FunctionTemplate>(New);
    functionTemplate->SetClassName(Nan::New("DIBuilder").ToLocalChecked());
    functionTemplate->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(functionTemplate, "createCompileUnit", DIBuilderWrapper::CreateCompileUnit);
    Nan::SetPrototypeMethod(functionTemplate, "createFile", DIBuilderWrapper::CreateFile);
    Nan::SetPrototypeMethod(functionTemplate, "finalize", DIBuilderWrapper::Finalize);

    auto constructorFunction = Nan::GetFunction(functionTemplate).ToLocalChecked();
    diBuilderConstructor().Reset(constructorFunction);

    Nan::Set(target, Nan::New("DIBuilder").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(DIBuilderWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("DIBuilder constructor needs to be called with new");
    }

    if (info.Length() != 1 || !ModuleWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("IRBuilder constructor needs either be called with context: LLVMContext or basicBlock: BasicBlock, insertBefore?: Instruction");
    }

    auto* module = ModuleWrapper::FromValue(info[0])->getModule();
    auto* diBuilder = new llvm::DIBuilder { *module };

    DIBuilderWrapper* wrapper = new DIBuilderWrapper { diBuilder };

    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(DIBuilderWrapper::CreateFile) {
    if (info.Length() != 2 || !info[0]->IsString() || !info[1]->IsString()) {
        return Nan::ThrowTypeError("createFile needs to be called with filename: string, directory: string");
    }

    auto* diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();

    std::string name = ToString(info[0]);
    std::string directory = ToString(info[1]);

    auto *file = diBuilder->createFile(name, directory);

    info.GetReturnValue().Set(DIFileWrapper::of(file));
}

NAN_METHOD(DIBuilderWrapper::CreateCompileUnit) {
    auto* diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();

    if (info.Length() < 4 || !info[0]->IsUint32() || !DIFileWrapper::isInstance(info[1]) || !info[2]->IsString() || !info[3]->IsBoolean()
        || (info.Length() > 4 && !info[4]->IsString())
        || (info.Length() > 5 && !info[5]->IsUint32())
        || info.Length() > 6) {
        return Nan::ThrowTypeError("createCompileUnit needs to be called with language: dwarf.SourceLanguage, file: DIFile, producer: string, isOptimized: boolean, flags?: string, runtimeVersion?: number");
    }

    unsigned lang = Nan::To<unsigned>(info[0]).FromJust();
    llvm::DIFile *file = DIFileWrapper::FromValue(info[1])->getDIFile();
    std::string producer = ToString(info[2]);
    bool isOptimized = Nan::To<bool>(info[3]).FromJust();

    std::string flags = "";
    unsigned runtimeVersion = 0;

    if (info.Length() > 4) {
        flags = ToString(info[4]);
    }

    if (info.Length() > 5) {
        isOptimized = Nan::To<unsigned>(info[5]).FromJust();
    }

    llvm::DICompileUnit *compileUnit = diBuilder->createCompileUnit(lang, file, producer, isOptimized, flags, runtimeVersion);
    return info.GetReturnValue().Set(DICompileUnitWrapper::of(compileUnit));
}

NAN_METHOD(DIBuilderWrapper::Finalize) {
    auto* diBuilder = DIBuilderWrapper::FromValue(info.Holder())->getDIBuilder();
    diBuilder->finalize();
}

llvm::DIBuilder *DIBuilderWrapper::getDIBuilder() {
    return this->diBuilder;
}
