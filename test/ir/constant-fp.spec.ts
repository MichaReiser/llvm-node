import * as llvm from "../../";
import { createModule } from "../test-utils";

describe("ir/constant-fp", () => {
  let context: llvm.LLVMContext;

  beforeEach(() => {
    ({ context } = createModule());
  });

  it("inherits from Constant", () => {
    const value = llvm.ConstantFP.get(context, 10);

    expect(value.isNullValue()).toBe(false);
  });

  it("inherits from Value", () => {
    const value = llvm.ConstantFP.get(context, 10);

    expect(value.name).toBeDefined();
  });

  describe("get", () => {
    const untypedGet = llvm.ConstantFP.get as any;
    it("creates a new double constant", () => {
      const value = llvm.ConstantFP.get(context, 10);

      expect(value.value).toBe(10);
    });

    it("creates a new fp of the given type", () => {
      const floatType = llvm.Type.getFloatTy(context);

      const value = llvm.ConstantFP.get(floatType, "3");

      expect(value.type).toEqual(floatType);
      expect(value).toBeInstanceOf(llvm.ConstantFP);
      expect((value as llvm.ConstantFP).value).toBe(3);
    });

    it("throws if not called with a context as first argument", () => {
      expect(() => untypedGet()).toThrowError("get called with illegal arguments");
      expect(() => untypedGet({}).toThrowError("get called with illegal arguments"));
    });

    it("throws if not called with a number as second argument", () => {
      expect(() => untypedGet(context, "test")).toThrowError("get called with illegal arguments");
    });
  });

  describe("getZeroValueForNegation", () => {
    it("returns the zero value for negation", () => {
      const nan = llvm.ConstantFP.getZeroValueForNegation(llvm.Type.getDoubleTy(context));

      expect(nan).toBeDefined();
      expect(nan).toBeInstanceOf(llvm.Constant);
    });
  });

  describe("getNegativeZero", () => {
    it("returns the negative zero value", () => {
      const nan = llvm.ConstantFP.getNegativeZero(llvm.Type.getDoubleTy(context));

      expect(nan).toBeDefined();
      expect(nan).toBeInstanceOf(llvm.Constant);
    });
  });

  describe("getNaN", () => {
    it("returns the nan value", () => {
      const nan = llvm.ConstantFP.getNaN(llvm.Type.getDoubleTy(context));

      expect(nan).toBeDefined();
    });
  });

  describe("getInfinity", () => {
    it("returns the infinity value", () => {
      const nan = llvm.ConstantFP.getInfinity(llvm.Type.getDoubleTy(context));

      expect(nan).toBeDefined();
    });

    it("returns the infinity value for negative", () => {
      const nan = llvm.ConstantFP.getInfinity(llvm.Type.getDoubleTy(context), true);

      expect(nan).toBeDefined();
    });
  });

  describe("value", () => {
    it("returns the value", () => {
      const value = llvm.ConstantFP.get(context, 10);

      expect(value.value).toBe(10);
    });

    it("returns NaN for a NaN value", () => {
      const value = llvm.ConstantFP.get(context, NaN);

      expect(value.value).toBeNaN();
    });
  });
});
