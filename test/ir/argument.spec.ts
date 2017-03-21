import * as llvm from "../../";

describe("Argument", () => {
    let context: llvm.LLVMContext;
    let module: llvm.Module;
    let sin: llvm.Function;

    beforeEach(() => {
        debugger;
        context = new llvm.LLVMContext();
        module = new llvm.Module("test", context);
        sin = llvm.Function.create(llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [llvm.Type.getDoubleTy(context)], false), llvm.LinkageTypes.ExternalLinkage, "sin", module);
    });

    describe("constructor", () => {
        it("throws if called without a type", () => {
            expect(() => new llvm.Argument()).toThrowError("Argument constructor requires: type: Type, name: string?, function: Function?");
            expect(() => new llvm.Argument("tes").toThrowError("Argument constructor requires: type: Type, name: string?, function: Function?"));
        });

        it("throws if the second argument is not a string", () => {
            expect(() => new llvm.Argument(llvm.Type.getVoidTy(context), 1)).toThrowError("Argument constructor requires: type: Type, name: string?, function: Function?");
        });

        it("throws if third argument is not a function", () => {
            expect(() => new llvm.Argument(llvm.Type.getVoidTy(context), "test", 1)).toThrowError("Argument constructor requires: type: Type, name: string?, function: Function?");
        });
    });

    it("inherits from value", () => {
        const arg = new llvm.Argument(llvm.Type.getDoubleTy(context), "valueArg");

        expect(arg.name).toBeDefined();

        arg.release();
    });

    it("has an argument number", () => {
        expect(sin.getArguments()[0].argumentNumber).toBe(0);
    });

    describe("parent", () => {
        it("returns the parent function", () => {
            expect(sin.getArguments()[0].parent).toEqual(sin);
        });

        it("is undefined if the argument has no parent", () => {
            const arg = new llvm.Argument(llvm.Type.getDoubleTy(context), "orphan");

            expect(arg.parent).not.toBeDefined();

            arg.release();
        });
    });
});
