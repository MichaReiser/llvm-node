test("exposes the llvm module", function () {
    var llvm = require("../index");
    expect(llvm).toBeDefined();
    expect(llvm.Module).toBeDefined();
});
