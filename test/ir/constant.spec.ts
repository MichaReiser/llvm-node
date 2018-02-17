import * as llvm from "../../";

describe("Constant", () => {
  let context: llvm.LLVMContext;
  let module: llvm.Module;

  beforeEach(() => {
    context = new llvm.LLVMContext();
    module = new llvm.Module("test", context);
  });

  it("inherits from value", () => {
    expect(llvm.Constant.getNullValue(llvm.Type.getDoubleTy(context)).name).toBeDefined();
  });

  describe("getNullValue", () => {
    it("returns 0 double value for double type", () => {
      const nullValue = llvm.Constant.getNullValue(llvm.Type.getDoubleTy(context)) as llvm.ConstantFP;
      expect(nullValue.value).toBe(0);
    });

    it("throws if type is not passed", () => {
      expect(() => (llvm.Constant.getNullValue as any)(12)).toThrowError(
        "getNullValue needs to be called with: type: Type"
      );
    });
  });

  describe("isNullValue", () => {
    it("returns true for a null value", () => {
      const nullValue = llvm.Constant.getNullValue(llvm.Type.getDoubleTy(context));
      expect(nullValue.isNullValue()).toBe(true);
    });

    it("returns false otherwise", () => {
      const nullValue = llvm.Constant.getAllOnesValue(llvm.Type.getDoubleTy(context));
      expect(nullValue.isNullValue()).toBe(false);
    });
  });

  describe("isOneValue", () => {
    it("returns true for a value with the byte representation 0x01 and false otherwise", () => {
      expect(llvm.ConstantInt.get(context, 1).isOneValue()).toBe(true);
      expect(llvm.ConstantInt.get(context, 2).isOneValue()).toBe(false);
    });
  });

  describe("isAllOnesValue", () => {
    it("returns true for a all one value", () => {
      const nullValue = llvm.Constant.getAllOnesValue(llvm.Type.getDoubleTy(context));
      expect(nullValue.isAllOnesValue()).toBe(true);
    });

    it("returns false otherwise", () => {
      const nullValue = llvm.Constant.getNullValue(llvm.Type.getDoubleTy(context));
      expect(nullValue.isAllOnesValue()).toBe(false);
    });

    it("throws if not called with a type", () => {
      expect(() => (llvm.Constant.getAllOnesValue as any)({})).toThrowError(
        "getAllOnesValue needs to be called with: type: Type"
      );
    });
  });
});
