import * as llvm from "../../";

describe("ir/linkage-types", () => {
  for (const key of Object.keys(llvm.LinkageTypes)) {
    it(`defines ${key}`, () => {
      expect((llvm.LinkageTypes as any)[key]).toBeDefined();
    });
  }
});
