//
// Created by Micha Reiser on 01.03.17.
//

#include "function-type.h"

NAN_MODULE_INIT(FunctionTypeWrapper::Init) {
    auto functionTypeObject = Nan::New<v8::Object>();
    auto typeObj = Nan::New<v8::Object>(getObjectWithStaticMethods());

    Nan::SetPrototype(functionTypeObject, typeObj);

    Nan::SetMethod(functionTypeObject, "get", FunctionTypeWrapper::get);
    Nan::SetMethod(functionTypeObject, "isValidReturnType", FunctionTypeWrapper::isValidReturnType);
    Nan::SetMethod(functionTypeObject, "isValidArgumentType", FunctionTypeWrapper::isValidArgumentType);

    Nan::Set(target, Nan::New("FunctionType").ToLocalChecked(), functionTypeObject);
}

NAN_METHOD(FunctionTypeWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("Constructor needs to be called with new");
    }

    if (info.Length() < 1 || !info[0]->IsExternal()) {
        return Nan::ThrowTypeError("Expected type pointer");
    }

    auto* functionType = static_cast<llvm::FunctionType*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new FunctionTypeWrapper { functionType };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}


llvm::FunctionType *FunctionTypeWrapper::functionType() {
    return static_cast<llvm::FunctionType*>(this->type);
}

v8::Local<v8::Object> FunctionTypeWrapper::Create(llvm::FunctionType *functionType) {
    Nan::EscapableHandleScope escapeScope {};

    v8::Local<v8::FunctionTemplate> functionTemplate = Nan::New(functionTypeTemplate());
    auto constructorFunction = Nan::GetFunction(functionTemplate).ToLocalChecked();
    v8::Local<v8::Value> argv[1] = { Nan::New<v8::External>(functionType) };
    v8::Local<v8::Object> object = Nan::NewInstance(constructorFunction, 1, argv).ToLocalChecked();

    return escapeScope.Escape(object);
}


NAN_METHOD(FunctionTypeWrapper::get) {
    if (info.Length() < 2 || !TypeWrapper::isInstance(info[0])
        || (!(info.Length() == 2 && info[1]->IsBoolean())
            && !(info.Length() == 3 && info[1]->IsArray() && info[2]->IsBoolean()))) {
        return Nan::ThrowTypeError("Get needs to be called with: result: Type, params: Type[], isVarArg?: boolean");
    }

    auto returnType = TypeWrapper::FromValue(info[0])->getType();
    llvm::FunctionType* functionType {};

    if (info.Length() == 2) {
        auto varArg = Nan::To<bool>(info[1]).ToChecked();

        functionType = llvm::FunctionType::get(returnType, varArg);
    } else {
        auto argumentTypesArray = v8::Array::Cast(*info[1]);
        std::vector<llvm::Type *> argumentTypes {argumentTypesArray->Length()};

        for (size_t i = 0; i < argumentTypesArray->Length(); ++i) {
            auto argumentTypeObject = argumentTypesArray->Get(i);
            if (!TypeWrapper::isInstance(argumentTypeObject)) {
                return Nan::ThrowTypeError("Expected Type in arguments array");
            }

            argumentTypes[i] = TypeWrapper::FromValue(argumentTypeObject)->getType();
        }

        auto varArg = Nan::To<bool>(info[2]).ToChecked();

        functionType = llvm::FunctionType::get(returnType, argumentTypes, varArg);
    }

    info.GetReturnValue().Set(FunctionTypeWrapper::Create(functionType));
}

NAN_METHOD(FunctionTypeWrapper::isValidReturnType) {
    if (info.Length() != 1) {
        return Nan::ThrowTypeError("isValidReturnType takes exactly one argument");
    }

    if (!TypeWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("a Type is required");
    }

    auto returnTypeWrapper = TypeWrapper::FromValue(info[0]);
    info.GetReturnValue().Set(Nan::New(llvm::FunctionType::isValidArgumentType(returnTypeWrapper->getType())));
}

NAN_METHOD(FunctionTypeWrapper::isValidArgumentType) {
    if (info.Length() != 1) {
        return Nan::ThrowTypeError("isValidArgumentType takes exactly one argument");
    }

    if (!TypeWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("a Type is required");
    }

    auto argumentTypeWrapper = TypeWrapper::FromValue(info[0]);
    info.GetReturnValue().Set(Nan::New(llvm::FunctionType::isValidArgumentType(argumentTypeWrapper->getType())));
}

NAN_GETTER(FunctionTypeWrapper::getReturnType) {
    auto* functionTypeWrapper = FunctionTypeWrapper::FromValue(info.Holder());
    auto* returnType = functionTypeWrapper->functionType()->getReturnType();
    info.GetReturnValue().Set(TypeWrapper::Create(returnType));
}

NAN_GETTER(FunctionTypeWrapper::isVarArg) {
    auto* functionTypeWrapper = FunctionTypeWrapper::FromValue(info.Holder());
    info.GetReturnValue().Set(Nan::New(functionTypeWrapper->functionType()->isVarArg()));
}

NAN_GETTER(FunctionTypeWrapper::getNumParams) {
    auto* functionTypeWrapper = FunctionTypeWrapper::FromValue(info.Holder());
    info.GetReturnValue().Set(Nan::New(functionTypeWrapper->functionType()->getNumParams()));
}

NAN_METHOD(FunctionTypeWrapper::getParams) {
    auto* functionTypeWrapper = FunctionTypeWrapper::FromValue(info.Holder());
    auto result = Nan::New<v8::Array>(functionTypeWrapper->functionType()->getNumParams());

    uint32_t i = 0;
    for (auto paramsIterator = functionTypeWrapper->functionType()->param_begin(); paramsIterator != functionTypeWrapper->functionType()->param_end(); ++paramsIterator) {
        result->Set(i, TypeWrapper::Create(*paramsIterator));
        ++i;
    }

    info.GetReturnValue().Set(result);
}

NAN_METHOD(FunctionTypeWrapper::getParamType) {
    if (info.Length() != 1 && !info[0]->IsUint32()) {
        return Nan::ThrowTypeError("A positive int is required");
    }
    auto* functionTypeWrapper = FunctionTypeWrapper::FromValue(info.Holder());

    unsigned index = Nan::To<unsigned>(info[0]).FromJust();
    if (index < functionTypeWrapper->functionType()->getNumParams()) {
        auto* paramType = functionTypeWrapper->functionType()->getParamType(index);

        info.GetReturnValue().Set(TypeWrapper::Create(paramType));
    } else {
        return Nan::ThrowRangeError("Index out of range");
    }
}

Nan::Persistent<v8::FunctionTemplate>& FunctionTypeWrapper::functionTypeTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(FunctionTypeWrapper::New);
        localTemplate->SetClassName(Nan::New("FunctionType").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("returnType").ToLocalChecked(), FunctionTypeWrapper::getReturnType);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("isVarArg").ToLocalChecked(), FunctionTypeWrapper::isVarArg);
        Nan::SetAccessor(localTemplate->InstanceTemplate(), Nan::New("numParams").ToLocalChecked(), FunctionTypeWrapper::getNumParams);
        Nan::SetPrototypeMethod(localTemplate, "getParams", FunctionTypeWrapper::getParams);
        Nan::SetPrototypeMethod(localTemplate, "getParamType", FunctionTypeWrapper::getParamType);

        v8::Local<v8::FunctionTemplate> localTypeTemplate = Nan::New(TypeWrapper::typeTemplate());
        localTemplate->Inherit(localTypeTemplate);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

bool FunctionTypeWrapper::isInstance(v8::Local<v8::Value> value) {
    return Nan::New(functionTypeTemplate())->HasInstance(value);
}

