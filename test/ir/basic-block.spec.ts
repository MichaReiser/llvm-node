import * as llvm from "../../";
import { createBuilderWithBlock, createModule } from "../test-utils";

describe("BasicBlock", () => {
  let context: llvm.LLVMContext;
  let module: llvm.Module;

  beforeEach(() => {
    ({ context, module } = createModule());
  });

  it("inherits from value", () => {
    const block = llvm.BasicBlock.create(context);

    expect(block.name).toBeDefined();
  });

  describe("create", () => {
    const untypedCreate = llvm.BasicBlock.create as any;
    it("throws if called without context", () => {
      expect(() => untypedCreate()).toThrowError(
        "BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?"
      );
      expect(() =>
        untypedCreate("tes").toThrowError(
          "BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?"
        )
      );
    });

    it("throws if the second argument is not a string", () => {
      expect(() => untypedCreate(context, 1)).toThrowError(
        "BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?"
      );
    });

    it("throws if third argument is not a function", () => {
      expect(() => untypedCreate(context, "test", 1)).toThrowError(
        "BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?"
      );
    });

    it("throws if the fourth argument is not a basic block", () => {
      expect(() => untypedCreate(context, "test", undefined, 1)).toThrowError(
        "BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?"
      );
    });

    it("creates a basic block", () => {
      const fn = llvm.Function.create(
        llvm.FunctionType.get(llvm.Type.getVoidTy(context), [], false),
        llvm.LinkageTypes.ExternalLinkage,
        "fn",
        module
      );
      const block = llvm.BasicBlock.create(context, "wrapper", fn);

      expect(block).toBeInstanceOf(llvm.BasicBlock);
      expect(fn.getBasicBlocks()).toEqual([block]);
    });

    it("inserts the basic block before the specified one", () => {
      const fn = llvm.Function.create(
        llvm.FunctionType.get(llvm.Type.getVoidTy(context), [], false),
        llvm.LinkageTypes.ExternalLinkage,
        "fn",
        module
      );
      const insertBeforeThis = llvm.BasicBlock.create(context, "wrapper", fn);

      const block = llvm.BasicBlock.create(context, "insertBefore", fn, insertBeforeThis);

      expect(fn.getBasicBlocks()).toEqual([block, insertBeforeThis]);
    });
  });

  describe("empty", () => {
    it("returns if the basic block is empty", () => {
      const block = llvm.BasicBlock.create(context);

      expect(block.empty).toBe(true);
    });

    it("returns false if the basic block is not empty", () => {
      const { basicBlock, builder } = createBuilderWithBlock();

      builder.createRetVoid();

      expect(basicBlock.empty).toBe(false);
    });
  });

  describe("context", () => {
    it("returns the llvm context", () => {
      const basicBlock = llvm.BasicBlock.create(context);

      expect(basicBlock.context).toEqual(context);
    });
  });

  describe("getTerminator", () => {
    it("returns undefined if this block has no terminator", () => {
      const basicBlock = llvm.BasicBlock.create(context);

      expect(basicBlock.getTerminator()).toBeUndefined();
    });

    it("returns the terminator value", () => {
      const { basicBlock, builder } = createBuilderWithBlock();

      const retVoid = builder.createRetVoid();

      expect(basicBlock.getTerminator()).toEqual(retVoid);
    });
  });

  describe("eraseFromParent", () => {
    it("erases this block from the parent fn and deletes it", () => {
      const { basicBlock, fn } = createBuilderWithBlock({ blockName: "entry" });

      basicBlock.eraseFromParent();

      expect(fn.getEntryBlock()).toBeNull();
    });
  });

  describe("parent", () => {
    it("returns the parent function", () => {
      const { fn, basicBlock } = createBuilderWithBlock();

      expect(basicBlock.parent).toEqual(fn);
    });

    it("is undefined if the block has no parent", () => {
      const block = llvm.BasicBlock.create(context);

      expect(block.parent).not.toBeDefined();
    });
  });
});
