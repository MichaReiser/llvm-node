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

typedef llvm::Value* (*BinaryOpFn)(llvm::IRBuilder<>& builder, llvm::Value*, llvm::Value*, const llvm::Twine&);
template<BinaryOpFn method>
NAN_METHOD(NANBinaryOperation) {
    if (info.Length() < 2 || !ValueWrapper::isInstance(info[0]) || !ValueWrapper::isInstance(info[1])
        || (info.Length() == 3 && !info[2]->IsString())
        || info.Length() > 3) {
        return Nan::ThrowTypeError("Binary operation needs to be called with: lhs: Value, rhs: Value, name: string?");
    }

    auto* lhs = ValueWrapper::FromValue(info[0])->getValue();
    auto* rhs = ValueWrapper::FromValue(info[1])->getValue();
    std::string name {};

    if (info.Length() == 3) {
        name = ToString(info[2]);
    }

    auto* wrapper = IRBuilderWrapper::FromValue(info.Holder());
    llvm::Value* value = method(wrapper->getIRBuilder(), lhs, rhs, name);

    info.GetReturnValue().Set(ValueWrapper::of(value));
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


NAN_MODULE_INIT(IRBuilderWrapper::Init) {
    v8::Local<v8::FunctionTemplate> functionTemplate = Nan::New<v8::FunctionTemplate>(New);
    functionTemplate->SetClassName(Nan::New("IRBuilder").ToLocalChecked());
    functionTemplate->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(functionTemplate, "setInsertionPoint", IRBuilderWrapper::SetInsertionPoint);
    Nan::SetPrototypeMethod(functionTemplate, "createAdd", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateAdd>>);
    Nan::SetPrototypeMethod(functionTemplate, "createAlloca", IRBuilderWrapper::CreateAlloca);
    Nan::SetPrototypeMethod(functionTemplate, "createAlignedLoad", IRBuilderWrapper::CreateAlignedLoad);
    Nan::SetPrototypeMethod(functionTemplate, "createAlignedStore", IRBuilderWrapper::CreateAlignedStore);
    Nan::SetPrototypeMethod(functionTemplate, "createBr", IRBuilderWrapper::CreateBr);
    Nan::SetPrototypeMethod(functionTemplate, "createCall", IRBuilderWrapper::CreateCall);
    Nan::SetPrototypeMethod(functionTemplate, "createCondBr", IRBuilderWrapper::CreateCondBr);
    Nan::SetPrototypeMethod(functionTemplate, "createFAdd", &NANBinaryOperation<ToBinaryOp<&llvm::IRBuilder<>::CreateFAdd>>);
    Nan::SetPrototypeMethod(functionTemplate, "createFCmpOGT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpOGT>>);
    Nan::SetPrototypeMethod(functionTemplate, "createFCmpOLE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpOLE>>);
    Nan::SetPrototypeMethod(functionTemplate, "createFCmpOLT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpOLT>>);
    Nan::SetPrototypeMethod(functionTemplate, "createFCmpOEQ", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpOEQ>>);
    Nan::SetPrototypeMethod(functionTemplate, "createFCmpULE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpULE>>);
    Nan::SetPrototypeMethod(functionTemplate, "createFCmpULT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpULT>>);
    Nan::SetPrototypeMethod(functionTemplate, "createFCmpUEQ", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFCmpUEQ>>);
    Nan::SetPrototypeMethod(functionTemplate, "createFPToSI", &IRBuilderWrapper::ConvertOperation<&llvm::IRBuilder<>::CreateFPToSI>);
    Nan::SetPrototypeMethod(functionTemplate, "createFDiv", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFDiv>>);
    Nan::SetPrototypeMethod(functionTemplate, "createFRem", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFRem>>);
    Nan::SetPrototypeMethod(functionTemplate, "createFSub", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateFSub>>);
    Nan::SetPrototypeMethod(functionTemplate, "createInBoundsGEP", IRBuilderWrapper::CreateInBoundsGEP);
    Nan::SetPrototypeMethod(functionTemplate, "createIntCast", IRBuilderWrapper::CreateIntCast);
    Nan::SetPrototypeMethod(functionTemplate, "createICmpSGT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpSGT>>);
    Nan::SetPrototypeMethod(functionTemplate, "createICmpSLT", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpSLT>>);
    Nan::SetPrototypeMethod(functionTemplate, "createICmpSLE", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateICmpSLE>>);
    Nan::SetPrototypeMethod(functionTemplate, "createICmpEQ", &NANBinaryOperation<ToBinaryOp<&llvm::IRBuilder<>::CreateICmpEQ>>);
    Nan::SetPrototypeMethod(functionTemplate, "createLoad", IRBuilderWrapper::CreateLoad);
    Nan::SetPrototypeMethod(functionTemplate, "createMul", &NANBinaryOperation<ToBinaryOp<&llvm::IRBuilder<>::CreateMul>>);
    Nan::SetPrototypeMethod(functionTemplate, "createPhi", IRBuilderWrapper::CreatePHI);
    Nan::SetPrototypeMethod(functionTemplate, "createRet", IRBuilderWrapper::CreateRet);
    Nan::SetPrototypeMethod(functionTemplate, "createRetVoid", IRBuilderWrapper::CreateRetVoid);
    Nan::SetPrototypeMethod(functionTemplate, "createSDiv", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateSDiv>>);
    Nan::SetPrototypeMethod(functionTemplate, "createSub", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateSub>>);
    Nan::SetPrototypeMethod(functionTemplate, "createSRem", &NANBinaryOperation<&ToBinaryOp<&llvm::IRBuilder<>::CreateSRem>>);
    Nan::SetPrototypeMethod(functionTemplate, "createSIToFP", &IRBuilderWrapper::ConvertOperation<&llvm::IRBuilder<>::CreateSIToFP>);
    Nan::SetPrototypeMethod(functionTemplate, "createStore", IRBuilderWrapper::CreateStore);
    Nan::SetPrototypeMethod(functionTemplate, "getInsertBlock", IRBuilderWrapper::GetInsertBlock);

    auto constructorFunction = Nan::GetFunction(functionTemplate).ToLocalChecked();
    irBuilderConstructor().Reset(constructorFunction);

    Nan::Set(target, Nan::New("IRBuilder").ToLocalChecked(), constructorFunction);
}

NAN_METHOD(IRBuilderWrapper::New) {
    if (!info.IsConstructCall()) {
        return Nan::ThrowTypeError("IRBuilder constructor needs to be called with new");
    }

    if (info.Length() < 1 || !(LLVMContextWrapper::isInstance(info[0]) || BasicBlockWrapper::isInstance(info[0]))) {
        return Nan::ThrowTypeError("IRBuilder constructor needs either be called with a context or a basic block");
    }

    IRBuilderWrapper* wrapper = nullptr;
    if (LLVMContextWrapper::isInstance(info[0])) {
        auto& llvmContext = LLVMContextWrapper::FromValue(info[0])->getContext();
        wrapper = new IRBuilderWrapper { llvm::IRBuilder<> { llvmContext } };
    } else {
        auto* basicBlock = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();
        wrapper = new IRBuilderWrapper { llvm::IRBuilder<> { basicBlock, basicBlock->begin() } };
    }

    wrapper->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
}

typedef llvm::Value* (llvm::IRBuilder<>::*ConvertOperationFn)(llvm::Value*, llvm::Type*, const llvm::Twine&);
template<ConvertOperationFn method>
NAN_METHOD(IRBuilderWrapper::ConvertOperation) {
    if (info.Length() < 2 || !ValueWrapper::isInstance(info[0]) || !TypeWrapper::isInstance(info[1])
        || (info.Length() == 3 && !info[2]->IsString())
        || info.Length() > 3) {
        return Nan::ThrowTypeError("Convert operation needs to be called with: value: Value, type: Type, name: string?");
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* type = TypeWrapper::FromValue(info[1])->getType();
    std::string name {};

    if (info.Length() == 3) {
        name = ToString(info[2]);
    }

    auto* wrapper = IRBuilderWrapper::FromValue(info.Holder());
    llvm::Value* returnValue = (wrapper->irBuilder.*method)(value, type, name);

    info.GetReturnValue().Set(ValueWrapper::of(returnValue));
}

NAN_METHOD(IRBuilderWrapper::CreateAlignedLoad) {
    if (info.Length() < 2 || !ValueWrapper::isInstance(info[0]) || !info[1]->IsUint32() ||
            (info.Length() == 3 && !info[2]->IsString()) ||
            info.Length() > 3) {
        return Nan::ThrowTypeError("createAlignedLoad needs to be called with: ptr: Value, alignment: uint32, name?: string");
    }

    auto* ptr = ValueWrapper::FromValue(info[0])->getValue();
    auto alignment = Nan::To<uint32_t>(info[1]).FromJust();
    std::string name {};

    if (info.Length() == 3) {
        name = ToString(info[2]);
    }

    auto* load = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateAlignedLoad(ptr, alignment, name);
    info.GetReturnValue().Set(ValueWrapper::of(load));
}

NAN_METHOD(IRBuilderWrapper::CreateAlignedStore) {
    if (info.Length() < 3 || !ValueWrapper::isInstance(info[0]) || !ValueWrapper::isInstance(info[1]) || !info[2]->IsUint32()
            || (info.Length() == 4 && !info[3]->IsBoolean())) {
        return Nan::ThrowTypeError("createAlignedStore needs to be called with: value: Value, ptr: Value, align: uint32, volatile?: boolean");
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* ptr = ValueWrapper::FromValue(info[1])->getValue();
    auto align = Nan::To<uint32_t>(info[2]).FromJust();
    bool isVolatile = false;

    if (info.Length() == 3) {
        isVolatile = Nan::To<bool>(info[3]).FromJust();
    }

    auto* store = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateAlignedStore(value, ptr, align, isVolatile);
    info.GetReturnValue().Set(ValueWrapper::of(store));
}

NAN_METHOD(IRBuilderWrapper::CreateAlloca) {
    if (info.Length() < 1 || !TypeWrapper::isInstance(info[0])
            || (info.Length() > 1 && !ValueWrapper::isInstance(info[1]) && !info[1]->IsUndefined())
            || (info.Length() > 2 && !info[2]->IsString() && !info[2]->IsUndefined())
            || info.Length() > 3) {
        return Nan::ThrowTypeError("createAlloca needs to be called with: type: Type, arraySize: Value, name?: string");
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    llvm::Value* value = nullptr;
    std::string name {};

    if (info.Length() > 1 && !info[1]->IsUndefined()) {
        value = ValueWrapper::FromValue(info[1])->getValue();
    }

    if (info.Length() > 2 && !info[2]->IsUndefined()) {
        name = ToString(Nan::To<v8::String>(info[2]).ToLocalChecked());
    }

    auto& irBuilder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;
    auto* alloc = irBuilder.CreateAlloca(type, value, name);

    info.GetReturnValue().Set(AllocaInstWrapper::of(alloc));
}

NAN_METHOD(IRBuilderWrapper::CreateInBoundsGEP) {
    if (info.Length() < 1 || !(ValueWrapper::isInstance(info[0]) || TypeWrapper::isInstance(info[0]))) {
        return Nan::ThrowTypeError("createInBoundsGEP needs to be called with: ptr: Value, idxList: Value[], name?: string or type: Type, ptr: Value, idxList: Value[], name?: string");
    }

    if (ValueWrapper::isInstance(info[0])) {
        return CreateInBoundsGEPWithoutType(info);
    }

    return CreateInBoundsGEPWithType(info);
}

Nan::NAN_METHOD_RETURN_TYPE IRBuilderWrapper::CreateInBoundsGEPWithType(Nan::NAN_METHOD_ARGS_TYPE info) {
    if (info.Length() < 3 || !TypeWrapper::isInstance(info[0]) || !ValueWrapper::isInstance(info[1]) || !info[2]->IsArray() ||
        (info.Length() == 4 && !info[3]->IsString() && !info[3]->IsUndefined())) {
        return Nan::ThrowTypeError("createInBoundsGEP needs to be called with: ptr: Value, idxList: Value[], name?: string or type: Type, ptr: Value, idxList: Value[], name?: string");
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto* ptr = ValueWrapper::FromValue(info[1])->getValue();
    auto indexValues = v8::Array::Cast(*info[2]);
    std::vector<llvm::Value*> idxList { indexValues->Length() };

    for (uint32_t i = 0; i < indexValues->Length(); ++i) {
        auto idx = indexValues->Get(i);

        if (!ValueWrapper::isInstance(idx)) {
            return Nan::ThrowTypeError("Value expected for idxList element");
        }

        idxList[i] = ValueWrapper::FromValue(idx)->getValue();
    }

    std::string name {};

    if (info.Length() == 4 && !info[3]->IsUndefined()) {
        name = ToString(info[3]);
    }

    auto* grep = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateInBoundsGEP(type, ptr, idxList, name);
    info.GetReturnValue().Set(ValueWrapper::of(grep));
}

Nan::NAN_METHOD_RETURN_TYPE IRBuilderWrapper::CreateInBoundsGEPWithoutType(Nan::NAN_METHOD_ARGS_TYPE info) {
    if (info.Length() < 2 || !ValueWrapper::isInstance(info[0]) || !info[1]->IsArray() ||
        (info.Length() == 3 && !info[2]->IsString() && !info[2]->IsUndefined())) {
        return Nan::ThrowTypeError("createInBoundsGEP needs to be called with: ptr: Value, idxList: Value[], name?: string or type: Type, ptr: Value, idxList: Value[], name?: string");
    }

    auto* ptr = ValueWrapper::FromValue(info[0])->getValue();
    auto indexValues = v8::Array::Cast(*info[1]);
    std::vector<llvm::Value*> idxList { indexValues->Length() };

    for (uint32_t i = 0; i < indexValues->Length(); ++i) {
        auto idx = indexValues->Get(i);

        if (!ValueWrapper::isInstance(idx)) {
            return Nan::ThrowTypeError("Value expected for idxList element");
        }

        idxList[i] = ValueWrapper::FromValue(idx)->getValue();
    }

    std::string name {};

    if (info.Length() == 3 && !info[2]->IsUndefined()) {
        name = ToString(info[2]);
    }

    auto* grep = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateInBoundsGEP(ptr, idxList, name);
    info.GetReturnValue().Set(ValueWrapper::of(grep));
}

NAN_METHOD(IRBuilderWrapper::CreateIntCast) {
    if (info.Length() < 3 || !ValueWrapper::isInstance(info[0]) || !TypeWrapper::isInstance(info[1]) || !info[2]->IsBoolean() ||
            (info.Length() == 4 && !info[3]->IsString()) ||
            info.Length() > 4) {
        return Nan::ThrowTypeError("createIntCast needs to be called with: value: Value, type: Type, isSigned: boolean, name?: string");
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* type = TypeWrapper::FromValue(info[1])->getType();
    auto isSigned = Nan::To<bool>(info[2]).ToChecked();
    std::string name {};

    if (info.Length() == 4) {
        name = ToString(info[3]);
    }

    auto* casted = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateIntCast(value, type, isSigned, name);
    info.GetReturnValue().Set(ValueWrapper::of(casted));
}

NAN_METHOD(IRBuilderWrapper::CreateLoad) {
    if (info.Length() < 1 || !ValueWrapper::isInstance(info[0])
            || (info.Length() > 1 && !info[1]->IsString())
            || info.Length() > 2) {
        return Nan::ThrowTypeError("createLoad needs to be called with: value: Value, name?: string");
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    std::string name {};

    if (info.Length() > 1) {
        name = ToString(Nan::To<v8::String>(info[1]).ToLocalChecked());
    }

    auto& irBuilder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;
    auto* inst = irBuilder.CreateLoad(value, name);
    info.GetReturnValue().Set(ValueWrapper::of(inst));
}

NAN_METHOD(IRBuilderWrapper::CreateStore) {
    if (info.Length() < 2 || !ValueWrapper::isInstance(info[0]) || !ValueWrapper::isInstance(info[1])
            || (info.Length() > 2 && !info[2]->IsBoolean())
            || info.Length() > 3) {
        return Nan::ThrowTypeError("createStore needs to be called with: value: Value, ptr: Value, isVolatile?: boolean");
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* ptr = ValueWrapper::FromValue(info[1])->getValue();
    bool isVolatile = false;

    if (info.Length() > 2) {
        isVolatile = Nan::To<bool>(info[2]).FromJust();
    }

    auto* inst = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateStore(value, ptr, isVolatile);
    info.GetReturnValue().Set(ValueWrapper::of(inst));
}

NAN_METHOD(IRBuilderWrapper::CreateCall) {
    if (info.Length() < 2 || !FunctionWrapper::isInstance(info[0]) || !info[1]->IsArray()
            || (info.Length() > 3 && !info[2]->IsString())
            || info.Length() > 3){
        return Nan::ThrowTypeError("createCall needs to be called with: callee: Function, args: Value[], name: string?");
    }

    auto* callee = FunctionWrapper::FromValue(info[0])->getFunction();
    auto* argsArray = v8::Array::Cast(*info[1]);
    std::vector<llvm::Value*> args { argsArray->Length() };
    std::string name {};

    for (uint32_t i = 0; i < argsArray->Length(); ++i) {
        if (!ValueWrapper::isInstance(argsArray->Get(i))) {
            return Nan::ThrowTypeError("Expected Value");
        }
        args[i] = ValueWrapper::FromValue(argsArray->Get(i))->getValue();
    }

    if (info.Length() == 3) {
        name = ToString(Nan::To<v8::String>(info[2]).ToLocalChecked());
    }

    auto* callInstr = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateCall(callee, args, name);
    info.GetReturnValue().Set(CallInstWrapper::of(callInstr));
}

NAN_METHOD(IRBuilderWrapper::CreatePHI) {
    if (info.Length() < 2 || !TypeWrapper::isInstance(info[0]) || !info[1]->IsUint32()
            || (info.Length() == 3 && !info[2]->IsString())
            || info.Length() > 3) {
        return Nan::ThrowTypeError("createPhi needs to be called with: type: Type, numReservedValues: uint32, name: string?");
    }

    auto* type = TypeWrapper::FromValue(info[0])->getType();
    auto numReservedValues = Nan::To<uint32_t>(info[1]).ToChecked();
    std::string name {};

    if (info.Length() == 3) {
        name = ToString(Nan::To<v8::String>(info[2]).ToLocalChecked());
    }

    auto* phi = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreatePHI(type, numReservedValues, name);
    info.GetReturnValue().Set(PhiNodeWrapper::of(phi));
}

NAN_METHOD(IRBuilderWrapper::CreateRet) {
    if (info.Length() != 1 || !ValueWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("createRet needs to be called with: value: Value");
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto& builder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;

    auto* returnInstruction = builder.CreateRet(value);
    info.GetReturnValue().Set(ValueWrapper::of(returnInstruction));
}

NAN_METHOD(IRBuilderWrapper::CreateRetVoid) {
    auto& builder = IRBuilderWrapper::FromValue(info.Holder())->irBuilder;
    auto* returnInstruction = builder.CreateRetVoid();
    info.GetReturnValue().Set(ValueWrapper::of(returnInstruction));
}

NAN_METHOD(IRBuilderWrapper::GetInsertBlock) {
    auto* builder = IRBuilderWrapper::FromValue(info.Holder());
    auto* block = builder->irBuilder.GetInsertBlock();
    info.GetReturnValue().Set(BasicBlockWrapper::of(block));
}

NAN_METHOD(IRBuilderWrapper::SetInsertionPoint) {
    if (info.Length() != 1 || !BasicBlockWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("setInsertionPoint needs to be called with: basicBlock: BasicBlock");
    }

    auto* basicBlock = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();
    auto* builder = IRBuilderWrapper::FromValue(info.Holder());

    builder->irBuilder.SetInsertPoint(basicBlock);
}

NAN_METHOD(IRBuilderWrapper::CreateBr) {
    if (info.Length() != 1 || !BasicBlockWrapper::isInstance(info[0])) {
        return Nan::ThrowTypeError("createBr needs to be called with: basicBlock: BasicBlock");
    }

    auto* basicBlock = BasicBlockWrapper::FromValue(info[0])->getBasicBlock();
    auto* builder = IRBuilderWrapper::FromValue(info.Holder());
    auto* branchInst = builder->irBuilder.CreateBr(basicBlock);

    info.GetReturnValue().Set(ValueWrapper::of(branchInst));
}

NAN_METHOD(IRBuilderWrapper::CreateCondBr) {
    if (info.Length() != 3 || !ValueWrapper::isInstance(info[0]) || !BasicBlockWrapper::isInstance(info[1]) || !BasicBlockWrapper::isInstance(info[2])) {
        return Nan::ThrowTypeError("createCondBr needs to be called with: condition: Value, then: BasicBlock, else: BasicBlock");
    }

    auto* value = ValueWrapper::FromValue(info[0])->getValue();
    auto* thenBlock = BasicBlockWrapper::FromValue(info[1])->getBasicBlock();
    auto* elseBlock = BasicBlockWrapper::FromValue(info[2])->getBasicBlock();

    auto* branchInst = IRBuilderWrapper::FromValue(info.Holder())->irBuilder.CreateCondBr(value, thenBlock, elseBlock);
    info.GetReturnValue().Set(ValueWrapper::of(branchInst));
}

llvm::IRBuilder<> &IRBuilderWrapper::getIRBuilder() {
    return this->irBuilder;
}
