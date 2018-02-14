import * as llvm from "../../";
import { createModule } from "../test-utils";

describe("ir/type", () => {
  describe("TypeID", () => {
    it("defines type ids", () => {
      expect(Object.keys(llvm.Type.TypeID).length).toBeGreaterThan(0);
    });

    for (const typeId of Object.keys(llvm.Type.TypeID)) {
      it(`defines ${typeId}`, () => {
        expect(llvm.Type.TypeID[typeId]).toBeDefined();
      });
    }
  });

  const { context } = createModule();

  test("doubleTy returns a type for which isDoubleTy is true", () => {
    const ty = llvm.Type.getDoubleTy(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isDoubleTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("voidTy returns a type for which isVoidTy is true", () => {
    const ty = llvm.Type.getVoidTy(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isDoubleTy()).toBe(false);
    expect(ty.isVoidTy()).toBe(true);
  });

  test("floatTy returns a type for which isFloatTy is true", () => {
    const ty = llvm.Type.getFloatTy(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isFloatTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("labelTy returns a type for which isLabelTy is true", () => {
    const ty = llvm.Type.getLabelTy(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isLabelTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("int1 returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getInt1Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("int8 returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getInt8Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("int16 returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getInt16Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("int32 returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getInt32Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("int64 returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getInt64Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("int128 returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getInt128Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("intN returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getIntNTy(context, 11);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("int1Ptr returns a type for which isPointerTy is true", () => {
    const ty = llvm.Type.getInt1PtrTy(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isPointerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("int8Ptr returns a type for which isPointerTy is true", () => {
    const ty = llvm.Type.getInt8PtrTy(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isPointerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("int32Ptr returns a type for which isPointerTy is true", () => {
    const ty = llvm.Type.getInt32PtrTy(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isPointerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("equals returns true if the two types are equal", () => {
    expect(llvm.Type.getInt32Ty(context).equals(llvm.Type.getInt32Ty(context))).toBe(true);
  });

  test("equals returns false when the two types are not equal", () => {
    expect(llvm.Type.getInt16Ty(context).equals(llvm.Type.getInt32Ty(context))).toBe(false);
  });

  xtest("isFunctionTy");
  xtest("isStructTy");
  xtest("isArrayTy");
  xtest("typeId");
  xtest("getPointerTo");
  xtest("getPrimitiveSizeInBits");
  xtest("toString");
});
