import * as llvm from "../../";

describe("ir/module", () => {
    let context: llvm.LLVMContext;

    beforeEach(() => {
        context = new llvm.LLVMContext();
    });

    describe("constructor", () => {
        test("can create a new module", () => {
            const mod = new llvm.Module("test", context);
            expect(mod).toBeDefined();
            expect(mod.moduleIdentifier).toBe("test");
        });

        test("throws if called with not enough arguments", () => {
            expect(() => new llvm.Module()).toThrowError("The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)");
            expect(() => new llvm.Module("test")).toThrowError("The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)");
        });

        test("throws with arguments not matching the expected type", () => {
           expect(() => new llvm.Module(1, context)).toThrowError("The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)");
           expect(() => new llvm.Module("test", {})).toThrowError("The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)");
        });
    });

    describe("moduleIdentifier", () => {
        test("sets the moduleIdentifier", () => {
            const mod = new llvm.Module("test", context);
            mod.moduleIdentifier = "fib";
            expect(mod.moduleIdentifier).toBe("fib");
        });

        test("it throws if it's not a string", () => {
            const mod = new llvm.Module("test", context);
            expect(() => mod.moduleIdentifier = 2).toThrowError("moduleIdentifier needs to be a string");
        });
    });

    describe("sourceFileName", () => {
        test("sets the source file name", () => {
            const mod = new llvm.Module("test", context);
            mod.sourceFileName = "fib.js";
            expect(mod.sourceFileName).toBe("fib.js");
        });

        test("it throws if it's not a string", () => {
            const mod = new llvm.Module("test", context);
            expect(() => mod.sourceFileName = 2).toThrowError("sourceFileName needs to be a string");
        });
    });

    describe("targetTriple", () => {
        test("sets the target triple", () => {
            const mod = new llvm.Module("test", context);
            mod.targetTriple = "wasm32-unknown-unknown";
            expect(mod.targetTriple).toBe("wasm32-unknown-unknown");
        });

        test("it throws if it's not a string", () => {
            const mod = new llvm.Module("test", context);
            expect(() => mod.targetTriple = 1).toThrowError("targetTriple needs to be a string");
        });

        it("it throws if it is an illegal target triple", () => {
            const mod = new llvm.Module("test", context);
            mod.targetTriple = "test";
        });
    });

    describe("dump", () => {
        test("dumps the module", () => {
            const mod = new llvm.Module("test", context);
            mod.dump();
        });
    });

    describe("getFunction", () => {
        test("returns the function with the given name", () => {
            const mod = new llvm.Module("test", context);

            const fun = llvm.Function.create(llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [], false), llvm.LinkageTypes.ExternalLinkage, "sin", mod);
            expect(mod.getFunction("sin")).toEqual(fun);
        });

        test("returns undefined if no such function exists", () => {
            const mod = new llvm.Module("test", context);

            expect(mod.getFunction("fib")).toBeUndefined();
        })
    });

    describe("print", () => {
        test("returns the llvm assembly code for the module", () => {
            const mod = new llvm.Module("test", context);
            const fun = llvm.Function.create(llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [], false), llvm.LinkageTypes.ExternalLinkage, "sin", mod);

            const entry = llvm.BasicBlock.create(context, "entry", fun);

            expect(mod.print()).toMatchSnapshot();
        });
    });
});