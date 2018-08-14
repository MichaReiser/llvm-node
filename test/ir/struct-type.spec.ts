import * as llvm from "../../";
import { createModule } from "../test-utils";

describe("ir/struct-type", () => {
  describe("create", () => {
    it("returns a struct type", () => {
      const { context } = createModule();

      const structType = llvm.StructType.create(context, "name");

      expect(structType).toBeInstanceOf(llvm.StructType);
      expect(structType.numElements).toBe(0);
    });
  });

  describe("get", () => {
    it("returns a struct type with the given elements", () => {
      const { context } = createModule();

      const structType = llvm.StructType.get(
        context,
        [llvm.Type.getDoubleTy(context), llvm.Type.getInt32Ty(context)],
        false
      );

      expect(structType).toBeInstanceOf(llvm.StructType);
      expect(structType.numElements).toBe(2);
      expect(structType.getElementType(0)).toEqual(llvm.Type.getDoubleTy(context));
      expect(structType.getElementType(1)).toEqual(llvm.Type.getInt32Ty(context));
    });
  });

  describe("name", () => {
    it("returns the name of the struct", () => {
      const { context } = createModule();

      const structType = llvm.StructType.create(context, "name");

      expect(structType.name).toBe("name");
    });

    it("returns undefined for literal struct types", () => {
      const { context } = createModule();

      const structType = llvm.StructType.get(context, [llvm.Type.getInt32Ty(context)]);

      expect(structType.name).toBeUndefined();
    });
  });

  describe("numElements", () => {
    it("returns the number of elements the struct has", () => {
      const { context } = createModule();

      const structType = llvm.StructType.get(
        context,
        [llvm.Type.getDoubleTy(context), llvm.Type.getInt32Ty(context)],
        false
      );

      expect(structType.numElements).toBe(2);
    });
  });

  describe("getElementType", () => {
    it("returns the type of the struct element", () => {
      const { context } = createModule();

      const structType = llvm.StructType.get(
        context,
        [llvm.Type.getDoubleTy(context), llvm.Type.getInt32Ty(context)],
        false
      );

      expect(structType.getElementType(0)).toEqual(llvm.Type.getDoubleTy(context));
      expect(structType.getElementType(1)).toEqual(llvm.Type.getInt32Ty(context));
    });
  });

  describe("setBody", () => {
    it("changes the elements of the struct", () => {
      const { context } = createModule();
      const structType = llvm.StructType.get(context, [llvm.Type.getInt32Ty(context)]);

      structType.setBody([llvm.Type.getDoubleTy(context), llvm.Type.getInt32Ty(context)]);

      expect(structType.numElements).toBe(2);
      expect(structType.getElementType(0)).toEqual(llvm.Type.getDoubleTy(context));
      expect(structType.getElementType(1)).toEqual(llvm.Type.getInt32Ty(context));
    });
  });
});
