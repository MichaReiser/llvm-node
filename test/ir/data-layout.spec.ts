import * as llvm from "../../";

describe("ir/data-layout", () => {
  function createDataLayout() {
    return new llvm.DataLayout("E-p:32:32-f128:128:128");
  }

  describe("getStringRepresentation", () => {
    it("returns the string representation", () => {
      const dataLayout = createDataLayout();

      expect(dataLayout.getStringRepresentation()).toEqual("E-p:32:32-f128:128:128");
    });
  });

  describe("getPointerSize", () => {
    it("returns the pointer size", () => {
      const dataLayout = createDataLayout();

      expect(dataLayout.getPointerSize(0)).toBe(4);
    });
  });

  describe("getPrefTypeAlignment", () => {
    it("returns the prefered type alignment", () => {
      const context = new llvm.LLVMContext();
      const dataLayout = createDataLayout();
      const f128 = llvm.Type.getFP128Ty(context);

      expect(dataLayout.getPrefTypeAlignment(f128)).toBe(16);
    });
  });

  describe("getTypeStoreSize", () => {
    it("returns the type store size", () => {
      const context = new llvm.LLVMContext();
      const dataLayout = createDataLayout();
      const f128 = llvm.Type.getFP128Ty(context);

      expect(dataLayout.getTypeStoreSize(f128)).toBe(16);
    });
  });

  describe("getIntPtrType", () => {
    it("returns the integer pointer type", () => {
      const context = new llvm.LLVMContext();
      const dataLayout = createDataLayout();

      const intPtrType = dataLayout.getIntPtrType(context, 0);

      expect(intPtrType).toEqual(llvm.Type.getInt32Ty(context));
    });
  });
});
