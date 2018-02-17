import * as llvm from "../../";

describe("ir/visibility-types", () => {
  it("defines Hidden, Default, and Protected", () => {
    expect(llvm.VisibilityTypes.Hidden).toBeDefined();
    expect(llvm.VisibilityTypes.Default).toBeDefined();
    expect(llvm.VisibilityTypes.Protected).toBeDefined();
  });
});
