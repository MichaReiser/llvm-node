import * as llvm from "../..";

describe("ir/llvm-context", () => {
  test("can create an instance", () => {
    const context = new llvm.LLVMContext();

    expect(context).toBeInstanceOf(llvm.LLVMContext);
  });
});
