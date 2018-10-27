import * as llvm from "../../";
import { createBuilder, createBuilderWithBlock } from "../test-utils";

describe("Value", () => {
  let context: llvm.LLVMContext;

  beforeEach(() => {
    context = new llvm.LLVMContext();
  });

  it("has the MaxAlignmentExponent set", () => {
    expect(llvm.Value.MaxAlignmentExponent).toBeDefined();
  });

  it("has the MaximumAlignment set", () => {
    expect(llvm.Value.MaximumAlignment).toBeDefined();
  });

  describe("name", () => {
    it("is empty by default", () => {
      const value = new llvm.Argument(llvm.Type.getDoubleTy(context));
      expect(value.name).toBe("");
    });

    it("sets the name", () => {
      const value = new llvm.Argument(llvm.Type.getDoubleTy(context));
      value.name = "testValue";

      expect(value.name).toEqual("testValue");
      expect(value.hasName()).toBe(true);
    });

    it("throws if the name is not a string", () => {
      const value = new llvm.Argument(llvm.Type.getDoubleTy(context));

      expect(() => (value.name = 1000 as any)).toThrowError("String required");
    });
  });

  describe("hasName", () => {
    it("returns false if the value has no name", () => {
      const value = new llvm.Argument(llvm.Type.getDoubleTy(context));
      expect(value.hasName()).toBe(false);
    });

    it("returns true if the value has a name", () => {
      const value = new llvm.Argument(llvm.Type.getDoubleTy(context), "name");
      expect(value.hasName()).toBe(true);
    });
  });

  describe("type", () => {
    it("returns the type of the value", () => {
      const doubleTy = llvm.Type.getDoubleTy(context);
      const value = new llvm.Argument(doubleTy);

      expect(value.type).toEqual(doubleTy);
    });
  });

  if (llvm.Value.prototype.dump) {
    describe("dump", () => {
      it("dumps the value", () => {
        const value = new llvm.Argument(llvm.Type.getDoubleTy(context));
        value.dump!();
      });
    });
  }

  describe("release", () => {
    it("releases the value", () => {
      const value = new llvm.Argument(llvm.Type.getInt32Ty(context));

      value.release();
    });
  });

  describe("deleteValue", () => {
    it("deletes the value", () => {
      const value = new llvm.Argument(llvm.Type.getInt32Ty(context));

      value.deleteValue();
    });
  });

  describe("replaceAllUsesWith", () => {
    it("replaces all uses with another value", () => {
      const { context, builder } = createBuilderWithBlock();
      const toReplace = builder.createAlloca(llvm.Type.getInt32Ty(context));
      builder.createStore(llvm.ConstantInt.get(context, 12), toReplace);
      const load = builder.createLoad(toReplace);

      const newAlloca = builder.createAlloca(llvm.Type.getInt32Ty(context));
      toReplace.replaceAllUsesWith(newAlloca);

      expect(toReplace.useEmpty()).toBe(true);
      expect(newAlloca.useEmpty()).toBe(false);
    });
  });

  describe("useEmpty", () => {
    it("returns false if the value is still in use", () => {
      const { context, builder } = createBuilderWithBlock();
      const alloca = builder.createAlloca(llvm.Type.getInt32Ty(context));
      builder.createStore(llvm.ConstantInt.get(context, 12), alloca);

      const load = builder.createLoad(alloca);

      expect(alloca.useEmpty()).toBe(false);
    });

    it("returns true if the value is not in use", () => {
      const { context, builder } = createBuilderWithBlock();
      const alloca = builder.createAlloca(llvm.Type.getInt32Ty(context));

      expect(alloca.useEmpty()).toBe(true);
    });
  });
});
