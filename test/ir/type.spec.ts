import * as llvm from "../../";
import { createModule } from "../test-utils";

describe("ir/type", () => {
  describe("TypeID", () => {
    it("defines type ids", () => {
      expect(Object.keys(llvm.Type.TypeID).length).toBeGreaterThan(0);
    });

    for (const typeId of Object.keys(llvm.Type.TypeID)) {
      it(`defines ${typeId}`, () => {
        expect((llvm.Type.TypeID as any)[typeId]).toBeDefined();
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

  test("FP128Ty returns a type for which isFP128Ty is true", () => {
    const ty = llvm.Type.getFP128Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isFP128Ty()).toBe(true);
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
    expect(ty).toBeInstanceOf(llvm.IntegerType);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
    expect(ty.getBitWidth()).toBe(1);
  });

  test("int8 returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getInt8Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty).toBeInstanceOf(llvm.IntegerType);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
    expect(ty.getBitWidth()).toBe(8);
  });

  test("int16 returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getInt16Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty).toBeInstanceOf(llvm.IntegerType);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
    expect(ty.getBitWidth()).toBe(16);
  });

  test("int32 returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getInt32Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty).toBeInstanceOf(llvm.IntegerType);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
    expect(ty.getBitWidth()).toBe(32);
  });

  test("int64 returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getInt64Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty).toBeInstanceOf(llvm.IntegerType);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
    expect(ty.getBitWidth()).toBe(64);
  });

  test("int128 returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getInt128Ty(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty).toBeInstanceOf(llvm.IntegerType);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("intN returns a type for which isIntegerTy is true", () => {
    const ty = llvm.Type.getIntNTy(context, 11);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty).toBeInstanceOf(llvm.IntegerType);
    expect(ty.isIntegerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
    expect(ty.getBitWidth()).toBe(11);
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

  test("getDoublePtrTy returns a type for which isPointerTy is true", () => {
    const ty = llvm.Type.getDoublePtrTy(context);

    expect(ty).toBeInstanceOf(llvm.Type);
    expect(ty.isPointerTy()).toBe(true);
    expect(ty.isVoidTy()).toBe(false);
  });

  test("getFloatPtrTy returns a type for which isPointerTy is true", () => {
    const ty = llvm.Type.getFloatPtrTy(context);

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

  test("isFunctionTy returns true for a function", () => {
    const fnType = llvm.FunctionType.get(llvm.Type.getVoidTy(context), false);

    expect(fnType.isFunctionTy()).toBe(true);
    expect(llvm.Type.getInt32Ty(context).isFunctionTy()).toBe(false);
  });

  test("isStructTy returns true for a struct", () => {
    const structTy = llvm.StructType.get(context, [llvm.Type.getInt32Ty(context)], false);

    expect(structTy.isStructTy()).toBe(true);
    expect(structTy.getElementType(0).isStructTy()).toBe(false);
  });

  test("isArrayTy returns true for an array", () => {
    const arrayTy = llvm.ArrayType.get(llvm.Type.getInt32Ty(context), 2);

    expect(arrayTy.isArrayTy()).toBe(true);
    expect(arrayTy.elementType.isArrayTy()).toBe(false);
  });

  test("typeId returns the id of the type", () => {
    expect(llvm.Type.getInt32Ty(context).typeID).toBe(llvm.Type.TypeID.IntegerTyID);
  });

  test("getPointerTo returns the pointer type of the type called on", () => {
    const type = llvm.Type.getDoubleTy(context);

    const pointerTy = type.getPointerTo();

    expect(pointerTy.isPointerTy()).toBe(true);
    expect(pointerTy.elementType).toEqual(llvm.Type.getDoubleTy(context));
  });

  test("getPrimitiveSizeInBits returns the size in bits", () => {
    expect(llvm.Type.getDoubleTy(context).getPrimitiveSizeInBits()).toBe(64);
    expect(llvm.Type.getInt32Ty(context).getPrimitiveSizeInBits()).toBe(32);
    expect(llvm.Type.getIntNTy(context, 14).getPrimitiveSizeInBits()).toBe(14);
  });

  test("toString returns the string representation of the type", () => {
    expect(
      llvm.Type.getDoubleTy(context)
        .getPointerTo()
        .toString()
    ).toBe("double*");
  });

  test("isHalfTy returns true for a half type", () => {
    expect(llvm.Type.getHalfTy(context).isHalfTy()).toBe(true);
  });

  test("isHalfTy returns false for a float", () => {
    expect(llvm.Type.getFloatTy(context).isHalfTy()).toBe(false);
  });
});
