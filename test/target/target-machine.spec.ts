import * as llvm from "../../";

describe("target/target-machine", () => {
  beforeAll(() => {
    llvm.initializeAllTargets();
    llvm.initializeAllTargetMCs();
    llvm.initializeAllTargetInfos();
  });

  test("create data layout returns a layout", () => {
    const target = llvm.TargetRegistry.lookupTarget("x86_64");
    const machine = target.createTargetMachine("x86_64-unknown-unknown", "generic");

    const dataLayout = machine.createDataLayout();

    expect(dataLayout).toBeInstanceOf(llvm.DataLayout);
  });
});
