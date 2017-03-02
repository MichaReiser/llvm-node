const llvm = require("../../index");

describe("BasicBlock", () => {
    let context: llvm.LLVMContext;
    let sin: llvm.Function;

    beforeEach(() => {
        context = new llvm.LLVMContext();
        const module = new llvm.Module("test", context);
        sin = llvm.Function.create(llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [llvm.Type.getDoubleTy(context)], false), llvm.LinkageTypes.ExternalLinkage, "sin", module);
    });

    describe("create", () => {
        it("throws if called without context", () => {
            expect(() => llvm.BasicBlock.create()).toThrowError("BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?");
            expect(() => llvm.BasicBlock.create("tes").toThrowError("BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?"));
        });

        it("throws if the second argument is not a string", () => {
            expect(() => llvm.BasicBlock.create(context, 1)).toThrowError("BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?");
        });

        it("throws if third argument is not a function", () => {
            expect(() => llvm.BasicBlock.create(context, "test", 1)).toThrowError("BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?");
        });

        it("throws if the fourth argument is not a basic block", () => {
            expect(() => llvm.BasicBlock.create(context, "test", undefined, 1)).toThrowError("BasicBlock.create needs to be called with: context: LLVMContext, name: string?, parent: Function?, insertBefore: BasicBlock?");
        });

        it("creates a basic block", () => {
            const insertBefore = llvm.BasicBlock.create(context, "wrapper", sin);

            expect(llvm.BasicBlock.create(context, "test", sin, insertBefore)).toBeDefined();
        })
    });

    it("inherits from value", () => {
        const block = llvm.BasicBlock.create(context);

        expect(block.name).toBeDefined();
    });

    describe("parent", () => {
        it("returns the parent function", () => {
            const block = llvm.BasicBlock.create(context, "entry", sin);

            expect(block.parent).toEqual(sin);
        });

        it("is undefined if the block has no parent", () => {
            const block = llvm.BasicBlock.create(context);

            expect(block.parent).not.toBeDefined();
        });
    });
});