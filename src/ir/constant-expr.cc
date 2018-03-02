#include "constant-expr.h"
#include "type.h"


NAN_MODULE_INIT(ConstantExprWrapper::Init) {
    auto constantExpr = Nan::GetFunction(Nan::New(constantExprTemplate())).ToLocalChecked();

    Nan::Set(target, Nan::New("ConstantExpr").ToLocalChecked(), constantExpr);
}

v8::Local<v8::Object> ConstantExprWrapper::of(llvm::ConstantExpr* constantExpr) {
    auto constructor = Nan::GetFunction(Nan::New(constantExprTemplate())).ToLocalChecked();
    v8::Local<v8::Value> args[1] = { Nan::New<v8::External> (constantExpr) };

    auto instance = Nan::NewInstance(constructor, 1, args).ToLocalChecked();

    Nan::EscapableHandleScope escapableHandleScope {};
    return escapableHandleScope.Escape(instance);
}

llvm::ConstantExpr* ConstantExprWrapper::getConstantExpr() {
    return static_cast<llvm::ConstantExpr*>(getValue());
}

Nan::Persistent<v8::FunctionTemplate>& ConstantExprWrapper::constantExprTemplate() {
    static Nan::Persistent<v8::FunctionTemplate> functionTemplate {};

    if (functionTemplate.IsEmpty()) {
        v8::Local<v8::FunctionTemplate> localTemplate = Nan::New<v8::FunctionTemplate>(ConstantExprWrapper::New);
        localTemplate->SetClassName(Nan::New("ConstantExpr").ToLocalChecked());
        localTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        localTemplate->Inherit(Nan::New(constantTemplate()));

        Nan::SetMethod(localTemplate, "getAlignOf", ConstantExprWrapper::getAlignOf);
        Nan::SetMethod(localTemplate, "getSizeOf", ConstantExprWrapper::getSizeOf);
        Nan::SetMethod(localTemplate, "getOr", ConstantExprWrapper::getOr);
        Nan::SetMethod(localTemplate, "getPointerBitCastOrAddrSpaceCast", ConstantExprWrapper::getPointerBitCastOrAddrSpaceCast);
        Nan::SetMethod(localTemplate, "getPointerCast", ConstantExprWrapper::getPointerCast);
        Nan::SetMethod(localTemplate, "getIntegerCast", ConstantExprWrapper::getIntegerCast);
        Nan::SetMethod(localTemplate, "getFPCast", ConstantExprWrapper::getFPCast);
        Nan::SetMethod(localTemplate, "getBitCast", ConstantExprWrapper::getBitCast);

        functionTemplate.Reset(localTemplate);
    }

    return functionTemplate;
}

NAN_METHOD(ConstantExprWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("ConstantExpr constructor needs to be called with new");
    }

    if (!info[0]->IsExternal()) {
        return Nan::ThrowTypeError("ConstantExpr constructor needs to be called with: constantExpr: External");
    }

    auto* constantExpr = static_cast<llvm::ConstantExpr*>(v8::External::Cast(*info[0])->Value());
    auto* wrapper = new ConstantExprWrapper { constantExpr };
    wrapper->Wrap(info.This());

    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(ConstantExprWrapper::getAlignOf) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("getAlignOf needs to be called with: type: Type");
    }

    auto type = TypeWrapper::FromValue(info[0])->getType();
    auto constantAlign = llvm::ConstantExpr::getAlignOf(type);

    info.GetReturnValue().Set(ConstantWrapper::of(constantAlign));
}

NAN_METHOD(ConstantExprWrapper::getSizeOf) {
    if (info.Length() != 1 || !TypeWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("getSizeOf needs to be called with: type: Type");
    }

    auto type = TypeWrapper::FromValue(info[0])->getType();
    auto constantSize = llvm::ConstantExpr::getSizeOf(type);

    info.GetReturnValue().Set(ConstantWrapper::of(constantSize));
}

NAN_METHOD(ConstantExprWrapper::getFPCast) {
    if (info.Length() != 2 || !ConstantWrapper::isInstance(info[0]) || !TypeWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("getFPCast needs to be called with: constant: Constant, type: Type");
    }

    auto constant = ConstantWrapper::FromValue(info[0])->getConstant();
    auto type = TypeWrapper::FromValue(info[1])->getType();

    auto constantCast = llvm::ConstantExpr::getFPCast(constant, type);

    info.GetReturnValue().Set(ConstantWrapper::of(constantCast));
}

NAN_METHOD(ConstantExprWrapper::getBitCast) {
    if (info.Length() != 2 || !ConstantWrapper::isInstance(info[0]) || !TypeWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("getBitCast needs to be called with: constant: Constant, type: Type");
    }

    auto constant = ConstantWrapper::FromValue(info[0])->getConstant();
    auto type = TypeWrapper::FromValue(info[1])->getType();

    auto constantCast = llvm::ConstantExpr::getBitCast(constant, type);

    info.GetReturnValue().Set(ConstantWrapper::of(constantCast));
}

NAN_METHOD(ConstantExprWrapper::getIntegerCast) {
    if (info.Length() != 3 || !ConstantWrapper::isInstance(info[0]) || !TypeWrapper::isInstance(info[1]) || !info[2]->IsBoolean()) {
        return Nan::ThrowTypeError("getIntegerCast needs to be called with: constant: Constant, type: Type, signed: Boolean");
    }

    auto constant = ConstantWrapper::FromValue(info[0])->getConstant();
    auto type = TypeWrapper::FromValue(info[1])->getType();
    bool isSigned = Nan::To<bool>(info[3]).FromJust();

    auto constantCast = llvm::ConstantExpr::getIntegerCast(constant, type, isSigned);

    info.GetReturnValue().Set(ConstantWrapper::of(constantCast));
}

NAN_METHOD(ConstantExprWrapper::getOr) {
    if (info.Length() != 2 || !ConstantWrapper::isInstance(info[0]) || !ConstantWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("getOr needs to be called with: constant1: Constant, constant2: Constant");
    }

    auto constant1 = ConstantWrapper::FromValue(info[0])->getConstant();
    auto constant2 = ConstantWrapper::FromValue(info[1])->getConstant();

    auto constant = llvm::ConstantExpr::getOr(constant1, constant2);

    info.GetReturnValue().Set(ConstantWrapper::of(constant));
}

NAN_METHOD(ConstantExprWrapper::getPointerBitCastOrAddrSpaceCast) {
    if (info.Length() != 2 || !ConstantWrapper::isInstance(info[0]) || !TypeWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("getPointerBitCastOrAddrSpaceCast needs to be called with: constant: Constant, type: Type");
    }


    auto constant = ConstantWrapper::FromValue(info[0])->getConstant();
    auto type = TypeWrapper::FromValue(info[1])->getType();

    auto constantBitCast = llvm::ConstantExpr::getPointerBitCastOrAddrSpaceCast(constant, type);

    info.GetReturnValue().Set(ConstantWrapper::of(constantBitCast));
}

NAN_METHOD(ConstantExprWrapper::getPointerCast) {
    if (info.Length() != 2 || !ConstantWrapper::isInstance(info[0]) || !TypeWrapper::isInstance(info[1])) {
        return Nan::ThrowTypeError("getPointerCast needs to be called with: constant: Constant, type: Type");
    }


    auto constant = ConstantWrapper::FromValue(info[0])->getConstant();
    auto type = TypeWrapper::FromValue(info[1])->getType();

    auto constantBitCast = llvm::ConstantExpr::getPointerCast(constant, type);

    info.GetReturnValue().Set(ConstantWrapper::of(constantBitCast));
}
