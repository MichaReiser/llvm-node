import * as llvm from "../../";

describe("ir/attribute", () => {
  it("initializes the AttrKind", () => {
    expect(llvm.Attribute.AttrKind).toBeDefined();
    expect(Object.keys(llvm.Attribute.AttrKind).length).toBeGreaterThan(0);
    for (const key of Object.keys(llvm.Attribute.AttrKind)) {
      expect((llvm.Attribute.AttrKind as any)[key]).toBeDefined();
    }
  });
});
