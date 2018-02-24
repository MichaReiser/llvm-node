//
// Created by Micha Reiser on 28.02.17.
//

#include "ir-builder.h"
#include "llvm-context.h"
#include "basic-block.h"
#include "function.h"
#include "type.h"
#include "phi-node.h"
#include "alloca-inst.h"
#include "call-inst.h"
#include "../util/array.h"
#include "function-type.h"

typedef llvm::Value* (*BinaryOpFn)(llvm::IRBuilder<>& builder, llvm::Value*, llvm::Value*, const llvm::Twine&);
template<BinaryOpFn method>
Napi::Value NANBinaryOperation(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !ValueWrapper::isInstance(info[0]) || !ValueWrapper::isInstance(info[1])
        || (info.Length() == 3 && !(info[2].IsString() || info[2].IsUndefined()))
        || info.Length() > 3) {
        Napi::TypeError::New(env, "Binary operation needs to be called with: lhs: Value, rhs: Value, name: string?").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* lhs = ValueWrapper::FromValue(info[0])->getValue();
    auto* rhs = ValueWrapper::FromValue(info[1])->getValue();
    std::string name {};

    if (info.Length() == 3 && !info[2].IsUndefined()) {
        name = ToString(info[2]);
    }

    auto* wrapper = IRBuilderWrapper::FromValue(info.Holder());
    llvm::Value* value = method(wrapper->getIRBuilder(), lhs, rhs, name);

    return ValueWrapper::of(value);
}

typedef llvm::Value* (llvm::IRBuilder<>::*BinaryOpFloatFn)(llvm::Value*, llvm::Value*, const llvm::Twine&, llvm::MDNode *FPMathTag);
template<BinaryOpFloatFn method>
llvm::Value* ToBinaryOp(llvm::IRBuilder<>& builder, llvm::Value* lhs, llvm::Value* rhs, const llvm::Twine& name) {
    return (builder.*method)(lhs, rhs, name, nullptr);
}

typedef llvm::Value* (llvm::IRBuilder<>::*BinaryOp)(llvm::Value*, llvm::Value*, const llvm::Twine&);
template<BinaryOp method>
llvm::Value* ToBinaryOp(llvm::IRBuilder<>& builder, llvm::Value* lhs, llvm::Value* rhs, const llvm::Twine& name) {
    llvm::Value* value = (builder.*method)(lhs, rhs, name);
    return value;
}

typedef llvm::Value* (llvm::IRBuilder<>::*BinaryOpWithOneBoolArg)(llvm::Value*, llvm::Value*, const llvm::Twine&, bool isExact);
template<BinaryOpWithOneBoolArg method>
llvm::Value* ToBinaryOp(llvm::IRBuilder<>& builder, llvm::Value* lhs, llvm::Value* rhs, const llvm::Twine& name) {
    llvm::Value* value = (builder.*method)(lhs, rhs, name, false);
    return value;
}

typedef llvm::Value* (llvm::IRBuilder<>::*BinaryIntOp)(llvm::Value*, llvm::Value*, const llvm::Twine&, bool HasNUW, bool HasNSW);
template<BinaryIntOp method>
llvm::Value* ToBinaryOp(llvm::IRBuilder<>& builder, llvm::Value* lhs, llvm::Value* rhs, const llvm::Twine& name) {
    llvm::Value* value = (builder.*method)(lhs, rhs, name, false, false);
    return value;
}

Napi::Object IRBuilderWrapper::Init(Napi::Env env, Napi::Object exports) {
    Napi::FunctionReference functionTemplate = Napi::Function::New(env, New);
    functionTemplate->SetClassName(Napi::String::New(env, "IRBuilder"));


    Napi::SetPrototypeMethod(functionTemplate, "createAdd", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateAdd>>);
    Napi::SetPrototypeMethod(functionTemplate, "createAlloca", IRBuilderWrapper::CreateAlloca);
    Napi::SetPrototypeMethod(functionTemplate, "createAlignedLoad", IRBuilderWrapper::CreateAlignedLoad);
    Napi::SetPrototypeMethod(functionTemplate, "createAlignedStore", IRBuilderWrapper::CreateAlignedStore);
    Napi::SetPrototypeMethod(functionTemplate, "createAnd", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateAnd>>);
    Napi::SetPrototypeMethod(functionTemplate, "createAShr", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateAShr>>);
    Napi::SetPrototypeMethod(functionTemplate, "createBitCast", IRBuilderWrapper::ConvertOperation<&llvm::IRBuilder<>::CreateBitCast>);
    Napi::SetPrototypeMethod(functionTemplate, "createBr", IRBuilderWrapper::CreateBr);
    Napi::SetPrototypeMethod(functionTemplate, "createCall", IRBuilderWrapper::CreateCall);
    Napi::SetPrototypeMethod(functionTemplate, "createCondBr", IRBuilderWrapper::CreateCondBr);
    Napi::SetPrototypeMethod(functionTemplate, "createExtractValue", IRBuilderWrapper::CreateExtractValue);
    Napi::SetPrototypeMethod(functionTemplate, "createFAdd", &NANBinaryOperation<ToBinaryOp<&llvm::IRBuilder<>::CreateFAdd>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpOGT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpOGT>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpOGE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpOGE>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpOLE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpOLE>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpOLT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpOLT>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpOEQ", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpOEQ>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpONE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpONE>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpUGT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpUGT>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpUGE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpUGE>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpULE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpULE>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpULT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpULT>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpUEQ", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpUEQ>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFCmpUNE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpUNE>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFDiv", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFDiv>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFMul", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFMul>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFNeg", IRBuilderWrapper::CreateFNeg);
    Napi::SetPrototypeMethod(functionTemplate, "createFPToSI", &IRBuilderWrapper::ConvertOperation<&llvm::IRBuilder<>::CreateFPToSI>);
    Napi::SetPrototypeMethod(functionTemplate, "createFRem", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFRem>>);
    Napi::SetPrototypeMethod(functionTemplate, "createFSub", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFSub>>);
    Napi::SetPrototypeMethod(functionTemplate, "createGlobalString", IRBuilderWrapper::CreateGlobalString);
    Napi::SetPrototypeMethod(functionTemplate, "createGlobalStringPtr", IRBuilderWrapper::CreateGlobalStringPtr);
    Napi::SetPrototypeMethod(functionTemplate, "createInBoundsGEP", IRBuilderWrapper::CreateInBoundsGEP);
    Napi::SetPrototypeMethod(functionTemplate, "createIntCast", IRBuilderWrapper::CreateIntCast);
    Napi::SetPrototypeMethod(functionTemplate, "createInsertValue", IRBuilderWrapper::CreateInsertValue);
    Napi::SetPrototypeMethod(functionTemplate, "createICmpEQ", &NANBinaryOperation<ToBinaryOp<&llvm::IRBuilder<>::CreateICmpEQ>>);
    Napi::SetPrototypeMethod(functionTemplate, "createICmpNE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpNE>>);
    Napi::SetPrototypeMethod(functionTemplate, "createICmpSGE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpSGE>>);
    Napi::SetPrototypeMethod(functionTemplate, "createICmpSGT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpSGT>>);
    Napi::SetPrototypeMethod(functionTemplate, "createICmpSLT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpSLT>>);
    Napi::SetPrototypeMethod(functionTemplate, "createICmpSLE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpSLE>>);
    Napi::SetPrototypeMethod(functionTemplate, "createICmpUGE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpUGE>>);
    Napi::SetPrototypeMethod(functionTemplate, "createICmpUGT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpUGT>>);
    Napi::SetPrototypeMethod(functionTemplate, "createICmpULT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpULT>>);
    Napi::SetPrototypeMethod(functionTemplate, "createICmpULE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpULE>>);
    Napi::SetPrototypeMethod(functionTemplate, "createLoad", IRBuilderWrapper::CreateLoad);
    Napi::SetPrototypeMethod(functionTemplate, "createLShr", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateLShr>>);
    Napi::SetPrototypeMethod(functionTemplate, "createMul", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateMul>>);
    Napi::SetPrototypeMethod(functionTemplate, "createNeg", IRBuilderWrapper::CreateNeg);
    Napi::SetPrototypeMethod(functionTemplate, "createNot", IRBuilderWrapper::CreateNot);
    Napi::SetPrototypeMethod(functionTemplate, "createOr", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateOr>>);
    Napi::SetPrototypeMethod(functionTemplate, "createXor", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateXor>>);
    Napi::SetPrototypeMethod(functionTemplate, "createPhi", IRBuilderWrapper::CreatePHI);
    Napi::SetPrototypeMethod(functionTemplate, "createPtrToInt", IRBuilderWrapper::ConvertOperation<&llvm::IRBuilder<>::CreatePtrToInt>);
    Napi::SetPrototypeMethod(functionTemplate, "createRet", IRBuilderWrapper::CreateRet);
    Napi::SetPrototypeMethod(functionTemplate, "createRetVoid", IRBuilderWrapper::CreateRetVoid);
    Napi::SetPrototypeMethod(functionTemplate, "createSDiv", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateSDiv>>);
    Napi::SetPrototypeMethod(functionTemplate, "createSelect", IRBuilderWrapper::CreateSelect);
    Napi::SetPrototypeMethod(functionTemplate, "createShl", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateShl>>);
    Napi::SetPrototypeMethod(functionTemplate, "createSub", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateSub>>);
    Napi::SetPrototypeMethod(functionTemplate, "createSRem", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateSRem>>);
    Napi::SetPrototypeMethod(functionTemplate, "createSIToFP", &IRBuilderWrapper::ConvertOperation<&llvm::IRBuilder<>::CreateSIToFP>);
    Napi::SetPrototypeMethod(functionTemplate, "createUIToFP", &IRBuilderWrapper::ConvertOperation<&llvm::IRBuilder<>::CreateUIToFP>);
    Napi::SetPrototypeMethod(functionTemplate, "createStore", IRBuilderWrapper::CreateStore);
    Napi::SetPrototypeMethod(functionTemplate, "createZExt", IRBuilderWrapper::ConvertOperation<&llvm::IRBuilder<>::CreateZExt>);
    Napi::SetPrototypeMethod(functionTemplate, "getInsertBlock", IRBuilderWrapper::GetInsertBlock);
    Napi::SetPrototypeMethod(functionTemplate, "setInsertionPoint", IRBuilderWrapper::SetInsertionPoint);

    auto constructorFunction = Napi::GetFunction(functionTemplate);
    irBuilderConstructor().Reset(constructorFunction);

    (target).Set(Napi::String::New(env, "IRBuilder"), constructorFunction);
}

Napi::Value IRBuilderWrapper::New(const Napi::CallbackInfo& info) {
    if (!info.IsConstructCall()) {
        Napi::TypeError::New(env, "IRBuilder constructor needs to be called with new").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (info.Length() < 1 || !(LLVMContextWrapper::isInstance(info[0]) || BasicBlockWrapper::isInstance(info[0]))
            || (info.Length() == 2 && !(ValueWrapper::isInstance(info[1]) || info[1].IsUndefined())) ||
            info.Length() > 2) {
        Napi::TypeError::New(env, "IRBuilder constructor needs either be called with context: LLVMContext or basicBlock: BasicBlock, insertBefore?: Instruction").ThrowAsJavaScriptException();
        return env.Null();
    }

    IRBuilderWrapper* wrapper = nullptr;
    if (LLVMContextWrapper::isInstance(info[0])) {
        auto& llvmContext = LLVMContextWrapper::FromValue(info[0])->getContext();
        wrapper = new IRBuilderWrapper { llvm::IRBuilder<> { llvmContext } };
    } else if (info.Length() == 1 || info[1].IsUndefined()){
        auto* basicBlock = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();
        wrapper = new IRBuilderWrapper { llvm::IRBuilder<> { basicBlock, basicBlock->begin() } };
    } else {
        auto* basicBlock = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();
        auto builder = llvm::IRBuilder<> { basicBlock };
        builder.SetInsertPoint(static_cast<llvm::Instruction*>(ValueWrapper::FromValue(info[1])->getValue()));
        wrapper = new IRBuilderWrapper { builder };
    }

    wrapper->Wrap(info.This());
    return info.This();
}

typedef llvm::Value* (llvm::IRBuilder<>::*ConvertOperationFn)(llvm::Value*, llvm::Type*, const llvm::Twine&);
template<ConvertOperationFn method>
Napi::Value IRBuilderWrapper::ConvertOperation(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !ValueWrapper::isInstance(info[0]) || !TypeWrapper::isInstance(info[1])
        || (info.Length() == 3 && !(info[2].IsString() || info[2].IsUndefined()))
        || info.Length() > 3) {
        Napi::TypeError::New(env, "Convert operation needs to be called with: value: Value, type: Type, name: string?").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* type = TypeWrapper::FromValue(info[1])->getType();
    std::string name {};

    if (info.Length() == 3 && !info[2].IsUndefined()) {
        name = ToString(info[2]);
    }

    auto* wrapper = IRBuilderWrapper::FromValue(info.Holder());
    llvm::Value* returnValue = (wrapper->irBuilder.*method)(value, type, name);

    return ValueWrapper::of(returnValue);
}

Napi::Value IRBuilderWrapper::CreateAlignedLoad(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !ValueWrapper::isInstance(info[0]) || !info[1].IsUint32() ||
            (info.Length() == 3 && !info[2].IsString() && !info[2].IsUndefined()) ||
            info.Length() > 3) {
        Napi::TypeError::New(env, "createAlignedLoad needs to be called with: ptr: Value, alignment: uint32, name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* ptr = ValueWrapper::FromValue(info[0])->getValue();
    auto alignment = info[1].As<Napi::Number>().Uint32Value();
    std::string name {};

    if (info.Length() == 3 && !info[2].IsUndefined()) {
        name = ToString(info[2]);
    }

    auto* load = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateAlignedLoad(ptr, alignment, name);
    return ValueWrapper::of(load);
}

Napi::Value IRBuilderWrapper::CreateAlignedStore(const Napi::CallbackInfo& info) {
    if (info.Length() < 3 || !ValueWrapper::isInstance(info[0]) || !ValueWrapper::isInstance(info[1]) || !info[2].IsUint32()
            || (info.Length() == 4 && !(info[3].IsBoolean() || info[3].IsUndefined()))) {
        Napi::TypeError::New(env, "createAlignedStore needs to be called with: value: Value, ptr: Value, align: uint32, volatile?: boolean").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* ptr = ValueWrapper::FromValue(info[1])->getValue();
    auto align = info[2].As<Napi::Number>().Uint32Value();
    bool isVolatile = false;

    if (info.Length() == 4 && !info[3].IsUndefined()) {
        isVolatile = info[3].As<Napi::Boolean>().Value();
    }

    auto* store = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateAlignedStore(value, ptr, align, isVolatile);
    return ValueWrapper::of(store);
}

Napi::Value IRBuilderWrapper::CreateAlloca(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !TypeWrapper::isInstance(info[0])
            || (info.Length() > 1 && !ValueWrapper::isInstance(info[1]) && !info[1].IsUndefined())
            || (info.Length() > 2 && !info[2].IsString() && !info[2].IsUndefined())
            || info.Length() > 3) {
        Napi::TypeError::New(env, "createAlloca needs to be called with: type: Type, arraySize: Value, name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    llvm::Value* value = nullptr;
    std::string name {};

    if (info.Length() > 1 && !info[1].IsUndefined()) {
        value = ValueWrapper::FromValue(info[1])->getValue();
    }

    if (info.Length() > 2 && !info[2].IsUndefined()) {
        name = ToString(info[2].To<Napi::String>());
    }

    auto& irBuilder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;
    auto* alloc = irBuilder.CreateAlloca(type, value, name);

    return AllocaInstWrapper::of(alloc);
}

Napi::Value IRBuilderWrapper::CreateExtractValue(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !ValueWrapper::isInstance(info[0]) || !info[1].IsArray() ||
        (info.Length() > 2 && !(info[2].IsString() || info[2].IsUndefined())) ||
        info.Length() > 3) {
        return Napi::ThrowTypeError(
                "createExtractValue needs to be called with: agg: Value, idxs: number[], name?: string");
    }

    auto& irBuilder = IRBuilderWrapper::FromValue(info.This())->getIRBuilder();
    auto* agg = ValueWrapper::FromValue(info[0])->getValue();
    auto idxs = toVector<unsigned>(info[1]);
    std::string name {};

    if (info.Length() > 2 && !info[2].IsUndefined()) {
        name = ToString(info[2]);
    }

    auto* value = irBuilder.CreateExtractValue(agg, idxs, name);

    return ValueWrapper::of(value);
}

Napi::Value IRBuilderWrapper::CreateInBoundsGEP(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !(ValueWrapper::isInstance(info[0]) || TypeWrapper::isInstance(info[0]))) {
        Napi::TypeError::New(env, "createInBoundsGEP needs to be called with: ptr: Value, idxList: Value[], name?: string or type: Type, ptr: Value, idxList: Value[], name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (ValueWrapper::isInstance(info[0])) {
        return CreateInBoundsGEPWithoutType(info);
    }

    return CreateInBoundsGEPWithType(info);
}

void IRBuilderWrapper::CreateInBoundsGEPWithType(const Napi::CallbackInfo& info) {
    if (info.Length() < 3 || !TypeWrapper::isInstance(info[0]) || !ValueWrapper::isInstance(info[1]) || !info[2].IsArray() ||
        (info.Length() == 4 && !info[3].IsString() && !info[3].IsUndefined())) {
        Napi::TypeError::New(env, "createInBoundsGEP needs to be called with: ptr: Value, idxList: Value[], name?: string or type: Type, ptr: Value, idxList: Value[], name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto* ptr = ValueWrapper::FromValue(info[1])->getValue();
    auto indexValues = *info[2].As<Napi::Array>();
    std::vector<llvm::Value*> idxList { indexValues->Length() };

    for (uint32_t i = 0; i < indexValues->Length(); ++i) {
        auto idx = indexValues->Get(i);

        if (!ValueWrapper::isInstance(idx)) {
            Napi::TypeError::New(env, "Value expected for idxList element").ThrowAsJavaScriptException();
            return env.Null();
        }

        idxList[i] = ValueWrapper::FromValue(idx)->getValue();
    }

    std::string name {};

    if (info.Length() == 4 && !info[3].IsUndefined()) {
        name = ToString(info[3]);
    }

    auto* grep = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateInBoundsGEP(type, ptr, idxList, name);
    return ValueWrapper::of(grep);
}

void IRBuilderWrapper::CreateInBoundsGEPWithoutType(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !ValueWrapper::isInstance(info[0]) || !info[1].IsArray() ||
        (info.Length() == 3 && !info[2].IsString() && !info[2].IsUndefined())) {
        Napi::TypeError::New(env, "createInBoundsGEP needs to be called with: ptr: Value, idxList: Value[], name?: string or type: Type, ptr: Value, idxList: Value[], name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* ptr = ValueWrapper::FromValue(info[0])->getValue();
    auto indexValues = *info[1].As<Napi::Array>();
    std::vector<llvm::Value*> idxList { indexValues->Length() };

    for (uint32_t i = 0; i < indexValues->Length(); ++i) {
        auto idx = indexValues->Get(i);

        if (!ValueWrapper::isInstance(idx)) {
            Napi::TypeError::New(env, "Value expected for idxList element").ThrowAsJavaScriptException();
            return env.Null();
        }

        idxList[i] = ValueWrapper::FromValue(idx)->getValue();
    }

    std::string name {};

    if (info.Length() == 3 && !info[2].IsUndefined()) {
        name = ToString(info[2]);
    }

    auto* grep = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateInBoundsGEP(ptr, idxList, name);
    return ValueWrapper::of(grep);
}

Napi::Value IRBuilderWrapper::CreateInsertValue(const Napi::CallbackInfo& info) {
    if (info.Length() < 3 || !ValueWrapper::isInstance(info[0]) || !ValueWrapper::isInstance(info[1]) || !info[2].IsArray() ||
            (info.Length() > 3 && !(info[3].IsString() || info[3].IsUndefined()))) {
        Napi::TypeError::New(env, "createInsertValue needs to be called with: agg: Value, val: Value, idx: unsigned[], name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto& builder = IRBuilderWrapper::FromValue(info.This())->getIRBuilder();
    auto* agg = ValueWrapper::FromValue(info[0])->getValue();
    auto* val = ValueWrapper::FromValue(info[1])->getValue();
    auto idx = toVector<unsigned>(info[2]);
    std::string name {};

    if (info.Length() > 3 && !info[3].IsUndefined()) {
        name = ToString(info[3]);
    }

    auto* inst = builder.CreateInsertValue(agg, val, idx, name);

    return ValueWrapper::of(inst);
}

Napi::Value IRBuilderWrapper::CreateIntCast(const Napi::CallbackInfo& info) {
    if (info.Length() < 3 || !ValueWrapper::isInstance(info[0]) || !TypeWrapper::isInstance(info[1]) || !info[2].IsBoolean() ||
            (info.Length() == 4 && !(info[3].IsString() || info[3].IsUndefined())) ||
            info.Length() > 4) {
        Napi::TypeError::New(env, "createIntCast needs to be called with: value: Value, type: Type, isSigned: boolean, name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* type = TypeWrapper::FromValue(info[1])->getType();
    auto isSigned = info[2].As<Napi::Boolean>().Value();
    std::string name {};

    if (info.Length() == 4 && !info[3].IsUndefined()) {
        name = ToString(info[3]);
    }

    auto* casted = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateIntCast(value, type, isSigned, name);
    return ValueWrapper::of(casted);
}

Napi::Value IRBuilderWrapper::CreateLoad(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !ValueWrapper::isInstance(info[0])
            || (info.Length() > 1 && !(info[1].IsString() || info[1].IsUndefined()))
            || info.Length() > 2) {
        Napi::TypeError::New(env, "createLoad needs to be called with: value: Value, name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    std::string name {};

    if (info.Length() > 1 && !info[1].IsUndefined()) {
        name = ToString(info[1].To<Napi::String>());
    }

    auto& irBuilder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;
    auto* inst = irBuilder.CreateLoad(value, name);
    return ValueWrapper::of(inst);
}

Napi::Value IRBuilderWrapper::CreateNeg(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !ValueWrapper::isInstance(info[0])
        || (info.Length() > 1 && !(info[1].IsString() || info[1].IsUndefined()))
        || (info.Length() > 2 && !(info[2].IsBoolean() || info[2].IsUndefined()))
        || (info.Length() > 3 && !(info[3].IsBoolean() || info[3].IsUndefined()))
        || info.Length() > 4) {
        Napi::TypeError::New(env, "createNeg needs to be called with: value: Value, name?: string, hasNuw?: boolean, hasNSW?: boolean").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    std::string name {};
    bool hasNUW {};
    bool hasNSW {};

    if (info.Length() > 1 && !info[1].IsUndefined()) {
        name = ToString(info[1].To<Napi::String>());
    }

    if (info.Length() > 2 && !info[2].IsUndefined()) {
        hasNUW = info[2].As<Napi::Boolean>().Value();
    }

    if (info.Length() > 3 && !info[3].IsUndefined()) {
        hasNSW = info[3].As<Napi::Boolean>().Value();
    }

    auto& irBuilder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;
    auto* inst = irBuilder.CreateNeg(value, name, hasNUW, hasNSW);
    return ValueWrapper::of(inst);
}

Napi::Value IRBuilderWrapper::CreateNot(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !ValueWrapper::isInstance(info[0])
        || (info.Length() == 2 && !(info[1].IsString() || info[2].IsUndefined()))
        || info.Length() > 2) {
        Napi::TypeError::New(env, "createNot needs to be called with: value: Value, name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    std::string name {};

    if (info.Length() == 2 && !info[1].IsUndefined()) {
        name = ToString(info[1].To<Napi::String>());
    }

    auto& irBuilder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;
    auto* inst = irBuilder.CreateNot(value, name);
    return ValueWrapper::of(inst);
}

Napi::Value IRBuilderWrapper::CreateStore(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !ValueWrapper::isInstance(info[0]) || !ValueWrapper::isInstance(info[1])
            || (info.Length() > 2 && !(info[2].IsBoolean() || info[2].IsUndefined()))
            || info.Length() > 3) {
        Napi::TypeError::New(env, "createStore needs to be called with: value: Value, ptr: Value, isVolatile?: boolean").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* ptr = ValueWrapper::FromValue(info[1])->getValue();
    bool isVolatile = false;

    if (info.Length() > 2 && !info[2].IsUndefined()) {
        isVolatile = info[2].As<Napi::Boolean>().Value();
    }

    auto* inst = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateStore(value, ptr, isVolatile);
    return ValueWrapper::of(inst);
}

Napi::Value IRBuilderWrapper::CreateCall(const Napi::CallbackInfo& info) {
    llvm::FunctionType* functionType {};
    llvm::Value* callee {};
    v8::Array* argsArray {};
    std::string name {};

    if (info.Length() > 1 && ValueWrapper::isInstance(info[0]) && info[1].IsArray() &&
            (info.Length() < 3 || info[2].IsUndefined() || info[2].IsString())) {
        callee = ValueWrapper::FromValue(info[0])->getValue();
        argsArray = *info[1].As<Napi::Array>();

        if (info.Length() == 3  && !info[2].IsUndefined()) {
            name = ToString(info[2].To<Napi::String>());
        }
    } else if (info.Length() > 1 && FunctionTypeWrapper::isInstance(info[0]) && ValueWrapper::isInstance(info[1]) && info[2].IsArray() &&
            (info.Length() < 4 || info[3].IsUndefined() || info[3].IsString())) {
        functionType = FunctionTypeWrapper::FromValue(info[0])->getFunctionType();
        callee = ValueWrapper::FromValue(info[1])->getValue();
        argsArray = *info[2].As<Napi::Array>();

        if (info.Length() == 4 && !info[3].IsUndefined()) {
            name = ToString(info[3].To<Napi::String>());
        }
    } else {
        Napi::TypeError::New(env, "createCall needs to be called with: callee: Value, args: Value[], name: string? or fty: FunctionType, callee: Value, args: ArrayRef, name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::vector<llvm::Value*> args { argsArray->Length() };

    for (uint32_t i = 0; i < argsArray->Length(); ++i) {
        if (!ValueWrapper::isInstance(argsArray->Get(i))) {
            Napi::TypeError::New(env, "Expected Value").ThrowAsJavaScriptException();
            return env.Null();
        }
        args[i] = ValueWrapper::FromValue(argsArray->Get(i))->getValue();
    }

    llvm::CallInst* callInst {};
    llvm::IRBuilder<>& irBuilder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;

    if (functionType == nullptr) {
        callInst = irBuilder.CreateCall(callee, args, name);
    } else {
        callInst = irBuilder.CreateCall(functionType, callee, args, name);
    }
    return CallInstWrapper::of(callInst);
}

Napi::Value IRBuilderWrapper::CreateGlobalString(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !info[0].IsString() ||
        (info.Length() > 1 && !(info[1].IsString() || info[1].IsUndefined())) ||
        (info.Length() == 3 && !(info[2].IsUint32() || info[2].IsUndefined())) ||
        info.Length() > 3) {
        Napi::TypeError::New(env, "createGlobalString needs to be called with: str: string, name?: string, addressSpace?: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto str = ToString(info[0]);
    std::string name {};
    uint32_t as = 0;

    if (info.Length() > 1 && !info[1].IsUndefined()) {
        name = ToString(info[1]);
    }

    if (info.Length() > 2 && !info[2].IsUndefined()) {
        as = info[2].As<Napi::Number>().Uint32Value();
    }

    auto* value = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateGlobalString(str, name, as);
    return ValueWrapper::of(value);
}

Napi::Value IRBuilderWrapper::CreateGlobalStringPtr(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !info[0].IsString() ||
            (info.Length() > 1 && !(info[1].IsString() || info[1].IsUndefined())) ||
            (info.Length() == 3 && !(info[2].IsUint32() || info[2].IsUndefined())) ||
            info.Length() > 3) {
        Napi::TypeError::New(env, "createGlobalStringPtr needs to be called with: str: string, name?: string, addressSpace?: uint32").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto str = ToString(info[0]);
    std::string name {};
    uint32_t as = 0;

    if (info.Length() > 1 && !info[1].IsUndefined()) {
        name = ToString(info[1]);
    }

    if (info.Length() > 2 && !info[2].IsUndefined()) {
        as = info[2].As<Napi::Number>().Uint32Value();
    }

    auto* value = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateGlobalStringPtr(str, name, as);
    return ValueWrapper::of(value);
}

Napi::Value IRBuilderWrapper::CreatePHI(const Napi::CallbackInfo& info) {
    if (info.Length() < 2 || !TypeWrapper::isInstance(info[0]) || !info[1].IsUint32()
            || (info.Length() == 3 && !(info[2].IsString() || info[2].IsUndefined()))
            || info.Length() > 3) {
        Napi::TypeError::New(env, "createPhi needs to be called with: type: Type, numReservedValues: uint32, name: string?").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto numReservedValues = info[1].As<Napi::Number>().Uint32Value();
    std::string name {};

    if (info.Length() == 3 && !info[2].IsUndefined()) {
        name = ToString(info[2].To<Napi::String>());
    }

    auto* phi = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreatePHI(type, numReservedValues, name);
    return PhiNodeWrapper::of(phi);
}

Napi::Value IRBuilderWrapper::CreateRet(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !ValueWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "createRet needs to be called with: value: Value").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto& builder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;

    auto* returnInstruction = builder.CreateRet(value);
    return ValueWrapper::of(returnInstruction);
}

Napi::Value IRBuilderWrapper::CreateRetVoid(const Napi::CallbackInfo& info) {
    auto& builder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;
    auto* returnInstruction = builder.CreateRetVoid();
    return ValueWrapper::of(returnInstruction);
}

Napi::Value IRBuilderWrapper::CreateSelect(const Napi::CallbackInfo& info) {
    if (info.Length() < 3 || !ValueWrapper::isInstance(info[0]) || !ValueWrapper::isInstance(info[1]) || !ValueWrapper::isInstance(info[2]) ||
            (info.Length() == 4 && !(info[3].IsString() || info[3].IsUndefined())) ||
            info.Length() > 4) {
        Napi::TypeError::New(env, "createSelect needs to be called with: condition: Value, true: Value, false: Value, name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto& builder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;
    auto* condition = ValueWrapper::FromValue(info[0])->getValue();
    auto* trueValue = ValueWrapper::FromValue(info[1])->getValue();
    auto* falseValue = ValueWrapper::FromValue(info[2])->getValue();
    std::string name {};

    if (info.Length() == 4 && !info[3].IsUndefined()) {
        name = ToString(info[3]);
    }

    return ValueWrapper::of(builder.CreateSelect(condition, trueValue, falseValue, name));
}

Napi::Value IRBuilderWrapper::GetInsertBlock(const Napi::CallbackInfo& info) {
    auto* builder = IRBuilderWrapper::FromValue(info.Holder());
    auto* block = builder->irBuilder.GetInsertBlock();
    return BasicBlockWrapper::of(block);
}

Napi::Value IRBuilderWrapper::SetInsertionPoint(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !BasicBlockWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "setInsertionPoint needs to be called with: basicBlock: BasicBlock").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* basicBlock = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();
    auto* builder = IRBuilderWrapper::FromValue(info.Holder());

    builder->irBuilder.SetInsertPoint(basicBlock);
}

Napi::Value IRBuilderWrapper::CreateBr(const Napi::CallbackInfo& info) {
    if (info.Length() != 1 || !BasicBlockWrapper::isInstance(info[0])) {
        Napi::TypeError::New(env, "createBr needs to be called with: basicBlock: BasicBlock").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* basicBlock = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();
    auto* builder = IRBuilderWrapper::FromValue(info.Holder());
    auto* branchInst = builder->irBuilder.CreateBr(basicBlock);

    return ValueWrapper::of(branchInst);
}

Napi::Value IRBuilderWrapper::CreateCondBr(const Napi::CallbackInfo& info) {
    if (info.Length() != 3 || !ValueWrapper::isInstance(info[0]) || !BasicBlockWrapper::isInstance(info[1]) || !BasicBlockWrapper::isInstance(info[2])) {
        Napi::TypeError::New(env, "createCondBr needs to be called with: condition: Value, then: BasicBlock, else: BasicBlock").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* thenBlock = BasicBlockWrapper::FromValue(info[1])->getBasicBlock();
    auto* elseBlock = BasicBlockWrapper::FromValue(info[2])->getBasicBlock();

    auto* branchInst = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateCondBr(value, thenBlock, elseBlock);
    return ValueWrapper::of(branchInst);
}

Napi::Value IRBuilderWrapper::CreateFNeg(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !ValueWrapper::isInstance(info[0]) ||
            (info.Length() == 2 && !(info[1].IsString() || info[1].IsUndefined()))) {
        Napi::TypeError::New(env, "createFNeg needs to be called with: value: Value, name?: string").ThrowAsJavaScriptException();
        return env.Null();
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    std::string name {};

    if (info.Length() == 2 && !info[1].IsUndefined()) {
        name = ToString(info[1]);
    }

    auto* neg = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateFNeg(value, name);
    return ValueWrapper::of(neg);
}

llvm::IRBuilder<> &IRBuilderWrapper::getIRBuilder() {
    return this->irBuilder;
}
