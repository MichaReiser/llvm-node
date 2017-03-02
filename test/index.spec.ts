test("exposes the llvm module", () => {
    const llvm = require("../index");
    expect(llvm).toBeDefined();
    expect(llvm.Module).toBeDefined();
});