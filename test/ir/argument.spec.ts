import * as llvm from "../../";
import { createModule } from "../test-utils";

describe("Argument", () => {
  let context: llvm.LLVMContext;
  let module: llvm.Module;

  beforeEach(() => {
    ({ context, module } = createModule());
  });

  function createSinFn() {
    return llvm.Function.create(
      llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [llvm.Type.getDoubleTy(context)], false),
      llvm.LinkageTypes.ExternalLinkage,
      "sin",
      module
    );
  }

  function createAddIntsFn() {
    const intTy = llvm.Type.getInt32Ty(context);
    return llvm.Function.create(
      llvm.FunctionType.get(intTy, [intTy, intTy], false),
      llvm.LinkageTypes.ExternalLinkage,
      "addInts",
      module
    );
  }

  describe("constructor", () => {
    it("throws if called without a type", () => {
      expect(() => new (llvm.Argument as any)()).toThrowError(
        "Argument constructor requires: type: Type, name: string?, function: Function?"
      );
      expect(() =>
        new (llvm.Argument as any)("tes").toThrowError(
          "Argument constructor requires: type: Type, name: string?, function: Function?"
        )
      );
    });

    it("throws if the second argument is not a string", () => {
      expect(() => new (llvm.Argument as any)(llvm.Type.getVoidTy(context), 1)).toThrowError(
        "Argument constructor requires: type: Type, name: string?, function: Function?"
      );
    });

    it("throws if third argument is not a function", () => {
      expect(() => new (llvm.Argument as any)(llvm.Type.getVoidTy(context), "test", 1)).toThrowError(
        "Argument constructor requires: type: Type, name: string?, function: Function?"
      );
    });
  });

  it("inherits from value", () => {
    const arg = new llvm.Argument(llvm.Type.getDoubleTy(context), "valueArg");

    expect(arg.name).toBeDefined();

    arg.release();
  });

  it("each argument has a unique number ", () => {
    const addInts = createAddIntsFn();

    expect(addInts.getArguments()).toEqual([
      expect.objectContaining({ argumentNumber: 0 }),
      expect.objectContaining({ argumentNumber: 1 })
    ]);
  });

  describe("parent", () => {
    it("returns the parent function", () => {
      const sin = createSinFn();

      expect(sin.getArguments()[0].parent).toEqual(sin);
    });

    it("is undefined if the argument has no parent", () => {
      const arg = new llvm.Argument(llvm.Type.getDoubleTy(context), "orphan");

      expect(arg.parent).not.toBeDefined();

      arg.release();
    });
  });

  describe("addAttr", () => {
    it("adds the attribute of the defined kind", () => {
      const sin = createSinFn();
      const valueArgument = sin.getArguments()[0];

      valueArgument.addAttr(llvm.Attribute.AttrKind.ReadOnly);

      expect(valueArgument.hasAttribute(llvm.Attribute.AttrKind.ReadOnly));
    });
  });

  describe("addDereferenceableAttr", () => {
    it("adds the dereferenceable attribute", () => {
      const fn = llvm.Function.create(
        llvm.FunctionType.get(llvm.Type.getVoidTy(context), [llvm.Type.getInt32Ty(context).getPointerTo()], false),
        llvm.LinkageTypes.ExternalWeakLinkage,
        undefined,
        module
      );
      const thisArgument = fn.getArguments()[0];

      thisArgument.addDereferenceableAttr(16);

      expect(thisArgument.hasAttribute(llvm.Attribute.AttrKind.Dereferenceable)).toBe(true);
    });
  });
});
