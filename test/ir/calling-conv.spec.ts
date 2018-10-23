import * as llvm from "../../";

describe("ir/calling-conv", () => {
  it("initializes the CallingConv enum", () => {
    expect(llvm.CallingConv).toBeDefined();
    expect(Object.keys(llvm.CallingConv).length).toBeGreaterThan(0);
    for (const key of Object.keys(llvm.CallingConv)) {
      expect((llvm.CallingConv as any)[key]).toBeDefined();
    }
  });
});
