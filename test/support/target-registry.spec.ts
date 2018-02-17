import * as llvm from "../../";

describe("support/target-registry", () => {
  beforeAll(() => {
    llvm.initializeAllTargets();
    llvm.initializeAllTargetInfos();
    llvm.initializeAllTargetMCs();
  });

  test("lookup target returns the target", () => {
    const target = llvm.TargetRegistry.lookupTarget("x86_64-unknown-unknown-unknown");

    expect(target).toBeDefined();
  });

  test("target name returns the name of the target", () => {
    const target = llvm.TargetRegistry.lookupTarget("x86_64");
    expect(target.name).toBe("x86-64");
  });

  test("short description returns a short description of the target", () => {
    const target = llvm.TargetRegistry.lookupTarget("x86_64");
    expect(target.shortDescription).toBeUndefined();
  });

  test("create target machine returns a TargetMachine", () => {
    const target = llvm.TargetRegistry.lookupTarget("x86_64");
    const machine = target.createTargetMachine("x86_64-generic-generic", "generic");

    expect(machine).toEqual(expect.any(Object));
  });
});
