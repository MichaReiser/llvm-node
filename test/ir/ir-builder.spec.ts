import * as llvm from "../../";
import { createBuilder, createBuilderWithBlock } from "../test-utils";

describe("IRBuilder", () => {
  test("createAdd returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const sum = builder.createAdd(
      llvm.ConstantInt.get(context, 1),
      llvm.ConstantInt.get(context, 2),
      "sum"
    );

    expect(sum).toBeInstanceOf(llvm.Value);
    expect(sum).toEqual(llvm.ConstantInt.get(context, 3));
  });

  test("createAlloca returns an allocation", () => {
    const { builder, context } = createBuilderWithBlock();

    const alloca = builder.createAlloca(
      llvm.Type.getInt32Ty(context),
      undefined,
      "test"
    );

    expect(alloca).toBeInstanceOf(llvm.AllocaInst);
    expect(alloca.name).toBe("test");
    expect((alloca as llvm.AllocaInst).allocatedType).toEqual(
      llvm.Type.getInt32Ty(context)
    );
  });

  test("createAlignedLoad returns a value", () => {
    const { builder, context } = createBuilderWithBlock();
    const alloca = builder.createAlloca(llvm.Type.getInt32Ty(context));

    const load = builder.createAlignedLoad(alloca, 4, "value");

    expect(load).toBeInstanceOf(llvm.Value);
    expect(load.name).toEqual("value");
  });

  test("createAlignedStore returns a value", () => {
    const { builder, context } = createBuilderWithBlock();
    const alloca = builder.createAlloca(llvm.Type.getInt32Ty(context));

    const store = builder.createAlignedStore(
      llvm.ConstantInt.get(context, 12),
      alloca,
      8
    );

    expect(store).toBeInstanceOf(llvm.Value);
  });

  test("createAnd returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const and = builder.createAnd(
      llvm.ConstantInt.get(context, 1),
      llvm.ConstantInt.get(context, 3)
    );

    expect(and).toBeInstanceOf(llvm.Value);
    expect(and).toEqual(llvm.ConstantInt.get(context, 1));
  });

  test("create AShr returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const ashr = builder.createAShr(
      llvm.ConstantInt.get(context, 4),
      llvm.ConstantInt.get(context, 1)
    );

    expect(ashr).toBeInstanceOf(llvm.Value);
    expect(ashr).toEqual(llvm.ConstantInt.get(context, 2));
  });

  test("createBitCast returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cast = builder.createBitCast(
      llvm.ConstantInt.get(context, 2.0, 64),
      llvm.Type.getInt8PtrTy(context)
    );

    expect(cast).toBeInstanceOf(llvm.Value);
    expect(cast.type).toEqual(
      llvm.PointerType.get(llvm.Type.getInt32Ty(context), 0)
    );
  });

  test("createBr returns a value", () => {
    const { builder, context, fn } = createBuilderWithBlock();

    const nextBlock = llvm.BasicBlock.create(context, "next", fn);
    const branch = builder.createBr(nextBlock);

    expect(branch).toBeInstanceOf(llvm.Value);
  });

  test("createCall returns a call inst", () => {
    const { builder, context } = createBuilderWithBlock();
    const fn = llvm.Function.create(
      llvm.FunctionType.get(llvm.Type.getVoidTy(context), [], false),
      llvm.LinkageTypes.ExternalWeakLinkage
    );

    const call = builder.createCall(fn, [], "call");

    expect(call).toBeInstanceOf(llvm.CallInst);
    expect(call.name).toBe("call");
  });

  test("createCondBr returns a value", () => {
    const { builder, context, fn } = createBuilderWithBlock();
    const condition = llvm.ConstantInt.getTrue(context);
    const whenTrue = llvm.BasicBlock.create(context, "whenTrue", fn);
    const whenFalse = llvm.BasicBlock.create(context, "whenFalse", fn);

    const br = builder.createCondBr(condition, whenTrue, whenFalse);

    expect(br).toBeInstanceOf(llvm.Value);
  });

  test("createExtractValue returns a value", () => {
    const { builder, context } = createBuilderWithBlock();
    const structType = llvm.StructType.get(context, [
      llvm.Type.getInt32Ty(context),
      llvm.Type.getFloatTy(context)
    ]);
    const struct = llvm.ConstantStruct.get(structType, [
      llvm.ConstantInt.get(context, 1),
      llvm.ConstantFP.get(context, 2.0)
    ]);

    const value = builder.createExtractValue(struct, [0], "firstvalue");

    expect(value).toBeInstanceOf(llvm.Value);
    expect(value).toEqual(llvm.ConstantInt.get(context, 1));
  });

  test("createFAdd returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const add = builder.createFAdd(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 2.0)
    );

    expect(add).toBeInstanceOf(llvm.Value);
    expect(add).toEqual(llvm.ConstantFP.get(context, 3.0));
  });

  test("createFCmpOGT returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpOGT(
      llvm.ConstantFP.get(context, 2.0),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpOGE returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpOGE(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpOLE returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpOLE(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpOLT returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpOLT(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 2.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpOEQ returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpOEQ(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpONE returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpONE(
      llvm.ConstantFP.get(context, 1.1),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpUGT returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpUGT(
      llvm.ConstantFP.get(context, 2.0),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpUGE returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpUGE(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpULE returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpULE(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpULT returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpULT(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 2.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpUGE returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpUGE(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpULE returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpULE(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpULT returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpULT(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 2.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpUEQ returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpUEQ(
      llvm.ConstantFP.get(context, 1.0),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFCmpUNE returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createFCmpUNE(
      llvm.ConstantFP.get(context, 2.0),
      llvm.ConstantFP.get(context, 1.0)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createFDiv returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const div = builder.createFDiv(
      llvm.ConstantFP.get(context, 10),
      llvm.ConstantFP.get(context, 2)
    );

    expect(div).toBeInstanceOf(llvm.Value);
    expect(div).toEqual(llvm.ConstantFP.get(context, 5));
  });

  test("createFMul returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const mul = builder.createFMul(
      llvm.ConstantFP.get(context, 2),
      llvm.ConstantFP.get(context, 5)
    );

    expect(mul).toBeInstanceOf(llvm.Value);
    expect(mul).toEqual(llvm.ConstantFP.get(context, 10));
  });

  test("createFNeg returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const neg = builder.createFNeg(llvm.ConstantFP.get(context, 2));

    expect(neg).toBeInstanceOf(llvm.Value);
    expect(neg).toEqual(llvm.ConstantFP.get(context, -2));
  });

  test("createFPToSI returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const si = builder.createFPToSI(
      llvm.ConstantFP.get(context, -2),
      llvm.Type.getInt32Ty(context)
    );

    expect(si).toBeInstanceOf(llvm.Value);
    expect(si).toEqual(llvm.ConstantInt.get(context, -2));
  });

  test("createFRem returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const rem = builder.createFRem(
      llvm.ConstantFP.get(context, 11),
      llvm.ConstantFP.get(context, 2)
    );

    expect(rem).toBeInstanceOf(llvm.Value);
    expect(rem).toEqual(llvm.ConstantFP.get(context, 1));
  });

  test("createFSub returns a value", () => {
    const { builder, context } = createBuilderWithBlock();

    const difference = builder.createFSub(
      llvm.ConstantFP.get(context, 10),
      llvm.ConstantFP.get(context, 2)
    );

    expect(difference).toBeInstanceOf(llvm.Value);
    expect(difference).toEqual(llvm.ConstantFP.get(context, 8));
  });

  test("createGlobalString returns a ptr to a string array", () => {
    const { builder } = createBuilderWithBlock();

    const arrayPtr = builder.createGlobalString("Hello world", "test", 0);

    expect(arrayPtr).toBeInstanceOf(llvm.Value);
  });

  test("createGlobalStringPtr returns a ptr to a string", () => {
    const { builder } = createBuilderWithBlock();

    const ptr = builder.createGlobalStringPtr("Hello world", "test", 0);

    expect(ptr).toBeInstanceOf(llvm.Value);
    expect(ptr.type.isPointerTy()).toBe(true);
  });

  test("createInBoundsGEP with type returns a value", () => {
    const { builder, context } = createBuilderWithBlock();
    const allocation = builder.createAlloca(
      llvm.ArrayType.get(llvm.Type.getInt32Ty(context), 5),
      llvm.ConstantInt.get(context, 5)
    );
    const zero = llvm.ConstantInt.get(context, 0);

    const gep = builder.createInBoundsGEP(
      allocation,
      [zero, zero],
      "firstElementPtr"
    );

    expect(gep).toBeInstanceOf(llvm.Value);
    expect(gep.name).toBe("firstElementPtr");
    expect(gep.type).toEqual(llvm.Type.getInt32Ty(context).getPointerTo());
  });

  test("createInBoundsGep with type returns a value", () => {
    const { builder, context } = createBuilderWithBlock();
    const arrayType = llvm.ArrayType.get(llvm.Type.getInt32Ty(context), 5);
    const allocation = builder.createAlloca(
      arrayType,
      llvm.ConstantInt.get(context, 5)
    );
    const zero = llvm.ConstantInt.get(context, 0);

    const gep = builder.createInBoundsGEP(
      arrayType,
      allocation,
      [zero, zero],
      "firstElementPtr"
    );

    expect(gep).toBeInstanceOf(llvm.Value);
    expect(gep.name).toBe("firstElementPtr");
    expect(gep.type).toEqual(llvm.Type.getInt32Ty(context).getPointerTo());
  });

  test("createIntCast", () => {
    const { builder, context } = createBuilderWithBlock();
    const value = llvm.ConstantInt.get(context, 34, 64);

    const cast = builder.createIntCast(
      value,
      llvm.Type.getInt32Ty(context),
      true,
      "signed"
    );

    expect(cast).toBeInstanceOf(llvm.Value);
    expect(cast.type).toEqual(llvm.Type.getInt32Ty(context));
  });

  test("createInsertValue", () => {
    const { builder, context } = createBuilderWithBlock();
    const structType = llvm.StructType.get(context, [
      llvm.Type.getInt32Ty(context),
      llvm.Type.getDoubleTy(context)
    ]);
    const structAlloca = builder.createAlloca(structType);

    const insertValue = builder.createInsertValue(
      structAlloca,
      llvm.ConstantInt.get(context, 2),
      [0]
    );

    expect(insertValue).toBeInstanceOf(llvm.Value);
  });

  test("createICmpEQ", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createICmpEQ(
      llvm.ConstantInt.get(context, 2),
      llvm.ConstantInt.get(context, 2)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createICmpNE", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createICmpNE(
      llvm.ConstantInt.get(context, 2),
      llvm.ConstantInt.get(context, 2)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getFalse(context));
  });

  test("createICmpSGE", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createICmpSGE(
      llvm.ConstantInt.get(context, 2),
      llvm.ConstantInt.get(context, 2)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createICmpSGT", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createICmpSGT(
      llvm.ConstantInt.get(context, 3),
      llvm.ConstantInt.get(context, 2)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createICmpSLT", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createICmpSLT(
      llvm.ConstantInt.get(context, 1),
      llvm.ConstantInt.get(context, 2)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createICmpSLE", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createICmpSLE(
      llvm.ConstantInt.get(context, 2),
      llvm.ConstantInt.get(context, 2)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createICmpUGE", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createICmpUGE(
      llvm.ConstantInt.get(context, 2),
      llvm.ConstantInt.get(context, 2)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createICmpUGT", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createICmpUGT(
      llvm.ConstantInt.get(context, 3),
      llvm.ConstantInt.get(context, 2)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createICmpULT", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createICmpULT(
      llvm.ConstantInt.get(context, 1),
      llvm.ConstantInt.get(context, 2)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createICmpULE", () => {
    const { builder, context } = createBuilderWithBlock();

    const cmp = builder.createICmpULE(
      llvm.ConstantInt.get(context, 2),
      llvm.ConstantInt.get(context, 2)
    );

    expect(cmp).toBeInstanceOf(llvm.Value);
    expect(cmp).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createLoad", () => {
    const { builder, context } = createBuilderWithBlock();
    const alloca = builder.createAlloca(llvm.Type.getInt32Ty(context));

    const load = builder.createLoad(alloca, "value");

    expect(load).toBeInstanceOf(llvm.Value);
    expect(load.name).toBe("value");
    expect(load.type).toEqual(llvm.Type.getInt32Ty(context));
  });

  test("createLShr", () => {
    const { builder, context } = createBuilderWithBlock();
    const value = llvm.ConstantInt.get(context, 10);

    const shifted = builder.createLShr(value, llvm.ConstantInt.get(context, 2));

    expect(shifted).toBeInstanceOf(llvm.Value);
    expect(shifted).toEqual(llvm.ConstantInt.get(context, 2));
  });

  test("createMul", () => {
    const { builder, context } = createBuilderWithBlock();

    const product = builder.createMul(
      llvm.ConstantInt.get(context, 2),
      llvm.ConstantInt.get(context, 3)
    );

    expect(product).toBeInstanceOf(llvm.Value);
    expect(product).toEqual(llvm.ConstantInt.get(context, 6));
  });

  test("createNeg", () => {
    const { builder, context } = createBuilderWithBlock();

    const neg = builder.createNeg(llvm.ConstantInt.get(context, 2));

    expect(neg).toBeInstanceOf(llvm.Value);
    expect(neg).toEqual(llvm.ConstantInt.get(context, -2));
  });

  test("createNot", () => {
    const { builder, context } = createBuilderWithBlock();

    const not = builder.createNot(llvm.ConstantInt.getTrue(context));

    expect(not).toBeInstanceOf(llvm.Value);
    expect(not).toEqual(llvm.ConstantInt.getFalse(context));
  });

  test("createOr", () => {
    const { builder, context } = createBuilderWithBlock();

    const or = builder.createOr(
      llvm.ConstantInt.getFalse(context),
      llvm.ConstantInt.getTrue(context)
    );

    expect(or).toBeInstanceOf(llvm.Value);
    expect(or).toEqual(llvm.ConstantInt.getTrue(context));
  });

  test("createXor", () => {
    const { builder, context } = createBuilderWithBlock();

    const xor = builder.createXor(
      llvm.ConstantInt.get(context, 5),
      llvm.ConstantInt.get(context, 6)
    );

    expect(xor).toBeInstanceOf(llvm.Value);
    expect(xor).toEqual(llvm.ConstantInt.get(context, 3));
  });

  test("createPhi", () => {
    const { builder, context } = createBuilderWithBlock();

    const phi = builder.createPhi(llvm.Type.getInt32Ty(context), 2, "phi");

    expect(phi).toBeInstanceOf(llvm.Value);
    expect(phi.name).toBe("phi");
    expect(phi.type).toEqual(llvm.Type.getInt32Ty(context));
  });

  test("createPtrToInt", () => {
    const { builder, context } = createBuilderWithBlock();
    const alloca = builder.createAlloca(llvm.Type.getInt32Ty(context));

    const ptrToInt = builder.createPtrToInt(
      alloca,
      llvm.Type.getInt32Ty(context),
      "name"
    );

    expect(ptrToInt).toBeInstanceOf(llvm.Value);
    expect(ptrToInt.name).toBe("name");
    expect(ptrToInt.type).toEqual(llvm.Type.getInt32Ty(context));
  });

  test("createRet", () => {
    const { builder, context } = createBuilderWithBlock();

    const ret = builder.createRet(llvm.ConstantInt.get(context, 0));

    expect(ret).toBeInstanceOf(llvm.Value);
  });

  test("createRetVoid", () => {
    const { builder, context } = createBuilderWithBlock();

    const retVoid = builder.createRetVoid();

    expect(retVoid).toBeInstanceOf(llvm.Value);
  });

  test("createSDiv", () => {
    const { builder, context } = createBuilderWithBlock();

    const div = builder.createSDiv(
      llvm.ConstantInt.get(context, 10),
      llvm.ConstantInt.get(context, 2)
    );

    expect(div).toBeInstanceOf(llvm.Value);
    expect(div).toEqual(llvm.ConstantInt.get(context, 5));
  });

  test("createSelect", () => {
    const { builder, context } = createBuilderWithBlock();

    const select = builder.createSelect(
      llvm.ConstantInt.getFalse(context),
      llvm.ConstantInt.get(context, 2),
      llvm.ConstantInt.get(context, 4),
      "name"
    );

    expect(select).toBeInstanceOf(llvm.Value);
    expect(select).toEqual(llvm.ConstantInt.get(context, 4));
  });

  test("createShl", () => {
    const { builder, context } = createBuilderWithBlock();

    const shl = builder.createShl(
      llvm.ConstantInt.get(context, 10),
      llvm.ConstantInt.get(context, 2)
    );

    expect(shl).toBeInstanceOf(llvm.Value);
    expect(shl).toEqual(llvm.ConstantInt.get(context, 40));
  });

  test("createSub", () => {
    const { builder, context } = createBuilderWithBlock();

    const sub = builder.createSub(
      llvm.ConstantInt.get(context, 10),
      llvm.ConstantInt.get(context, 4)
    );

    expect(sub).toBeInstanceOf(llvm.Value);
    expect(sub).toEqual(llvm.ConstantInt.get(context, 6));
  });

  test("createSRem", () => {
    const { builder, context } = createBuilderWithBlock();

    const rem = builder.createSRem(
      llvm.ConstantInt.get(context, 11),
      llvm.ConstantInt.get(context, 2)
    );

    expect(rem).toBeInstanceOf(llvm.Value);
    expect(rem).toEqual(llvm.ConstantInt.get(context, 1));
  });

  test("CreateURem", () => {
    const { builder, context } = createBuilderWithBlock();

    const rem = builder.CreateURem(
      llvm.ConstantInt.get(context, 11),
      llvm.ConstantInt.get(context, 2)
    );

    expect(rem).toBeInstanceOf(llvm.Value);
    expect(rem).toEqual(llvm.ConstantInt.get(context, 1));
  });

  test("createSITToFP", () => {
    const { builder, context } = createBuilderWithBlock();

    const fp = builder.createSIToFP(
      llvm.ConstantInt.get(context, -2),
      llvm.Type.getDoubleTy(context)
    );

    expect(fp).toBeInstanceOf(llvm.Value);
    expect(fp).toEqual(llvm.ConstantFP.get(context, -2));
  });

  test("createUITToP", () => {
    const { builder, context } = createBuilderWithBlock();

    const fp = builder.createUIToFP(
      llvm.ConstantInt.get(context, 10, 32, false),
      llvm.Type.getDoubleTy(context)
    );

    expect(fp).toBeInstanceOf(llvm.Value);
    expect(fp).toEqual(llvm.ConstantFP.get(context, 10));
  });

  test("createStore", () => {
    const { builder, context } = createBuilderWithBlock();
    const alloca = builder.createAlloca(llvm.Type.getInt32Ty(context));

    const store = builder.createStore(llvm.ConstantInt.get(context, 2), alloca);

    expect(store).toBeInstanceOf(llvm.Value);
  });

  test("createZExt", () => {
    const { builder, context } = createBuilderWithBlock();

    const zext = builder.createZExt(
      llvm.ConstantInt.get(context, 2, 32),
      llvm.Type.getInt64Ty(context)
    );

    expect(zext).toBeInstanceOf(llvm.Value);
    expect(zext.type).toEqual(llvm.Type.getInt64Ty(context));
  });

  test("getInsertBlock / setInsertionPoint", () => {
    const { builder, context, fn } = createBuilderWithBlock();
    const block = llvm.BasicBlock.create(context, "test", fn);
    builder.setInsertionPoint(block);

    expect(builder.getInsertBlock()).toEqual(block);
  });

  test("getInsertBlock returns undefined if insertion point has not been set", () => {
    const { builder } = createBuilder();

    expect(builder.getInsertBlock()).toBeUndefined();
  });
});
