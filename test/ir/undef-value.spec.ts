import * as llvm from "../../";
import { createModule } from "../test-utils";

describe("ir/undef-value", () => {
  test("get returns an undef value", () => {
    const { context } = createModule();
    const value = llvm.UndefValue.get(llvm.Type.getInt32Ty(context));

    expect(value).toBeInstanceOf(llvm.UndefValue);
  });
});
