import * as llvm from "../..";

describe("ir/function-type", () => {
  it("inherits type", () => {
    const context = new llvm.LLVMContext();

    const type = llvm.FunctionType.get(llvm.Type.getInt32Ty(context), false);

    expect(type.typeID).toBe(llvm.Type.TypeID.FunctionTyID);
  });

  describe("returnType", () => {
    it("returns the return type of the function", () => {
      const context = new llvm.LLVMContext();
      const returnType = llvm.Type.getInt32Ty(context);

      const functionType = llvm.FunctionType.get(returnType, false);

      expect(functionType.returnType).toEqual(returnType);
    });
  });

  describe("isVarArg", () => {
    it("returns true for vararg function types", () => {
      const context = new llvm.LLVMContext();
      const returnType = llvm.Type.getInt32Ty(context);

      expect(llvm.FunctionType.get(returnType, false).isVarArg).toBe(false);
      expect(llvm.FunctionType.get(returnType, true).isVarArg).toBe(true);

      expect(llvm.FunctionType.get(returnType, [], false).isVarArg).toBe(false);
      expect(llvm.FunctionType.get(returnType, [], true).isVarArg).toBe(true);
    });
  });

  describe("numParams", () => {
    it("returns 0 for a function without params", () => {
      const context = new llvm.LLVMContext();
      const functionType = llvm.FunctionType.get(llvm.Type.getInt32Ty(context), false);

      expect(functionType.numParams).toBe(0);
    });

    it("returns the number of params the function has", () => {
      const context = new llvm.LLVMContext();
      const functionType = llvm.FunctionType.get(
        llvm.Type.getInt32Ty(context),
        [llvm.Type.getInt1Ty(context), llvm.Type.getDoubleTy(context)],
        false
      );

      expect(functionType.numParams).toBe(2);
    });
  });

  describe("isValidReturnType", () => {
    it("returns true for a valid return type", () => {
      const context = new llvm.LLVMContext();
      expect(llvm.FunctionType.isValidReturnType(llvm.Type.getInt128Ty(context))).toBe(true);
    });

    it("returns false for an invalid return type", () => {
      const context = new llvm.LLVMContext();
      expect(llvm.FunctionType.isValidReturnType(llvm.FunctionType.get(llvm.Type.getInt16Ty(context), false))).toBe(
        false
      );
    });
  });

  describe("isValidArgumentType", () => {
    it("returns true for a valid return type", () => {
      const context = new llvm.LLVMContext();
      expect(llvm.FunctionType.isValidArgumentType(llvm.Type.getInt128Ty(context))).toBe(true);
    });

    it("returns false for an invalid return type", () => {
      const context = new llvm.LLVMContext();
      expect(llvm.FunctionType.isValidArgumentType(llvm.FunctionType.get(llvm.Type.getInt16Ty(context), false))).toBe(
        false
      );
    });
  });

  describe("getParams", () => {
    it("returns the params of the function type", () => {
      const context = new llvm.LLVMContext();
      const functionType = llvm.FunctionType.get(
        llvm.Type.getInt32Ty(context),
        [llvm.Type.getDoubleTy(context), llvm.Type.getVoidTy(context)],
        false
      );

      expect(functionType.getParams()).toEqual([llvm.Type.getDoubleTy(context), llvm.Type.getVoidTy(context)]);
    });
  });

  describe("getParamType", () => {
    it("returns the parameter type at the given position", () => {
      const context = new llvm.LLVMContext();
      const functionType = llvm.FunctionType.get(
        llvm.Type.getInt32Ty(context),
        [llvm.Type.getDoubleTy(context), llvm.Type.getVoidTy(context)],
        false
      );

      expect(functionType.getParamType(0)).toEqual(llvm.Type.getDoubleTy(context));
      expect(functionType.getParamType(1)).toEqual(llvm.Type.getVoidTy(context));
    });
  });
});
