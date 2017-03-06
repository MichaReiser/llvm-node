var llvm = require("../../index");
describe("ir/module", function () {
    var context;
    beforeEach(function () {
        context = new llvm.LLVMContext();
    });
    describe("constructor", function () {
        test("can create a new module", function () {
            var mod = new llvm.Module("test", context);
            expect(mod).toBeDefined();
            expect(mod.moduleIdentifier).toBe("test");
        });
        test("throws if called with not enough arguments", function () {
            expect(function () { return new llvm.Module(); }).toThrowError("The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)");
            expect(function () { return new llvm.Module("test"); }).toThrowError("The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)");
        });
        test("throws with arguments not matching the expected type", function () {
            expect(function () { return new llvm.Module(1, context); }).toThrowError("The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)");
            expect(function () { return new llvm.Module("test", {}); }).toThrowError("The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)");
        });
    });
    describe("moduleIdentifier", function () {
        test("sets the moduleIdentifier", function () {
            var mod = new llvm.Module("test", context);
            mod.moduleIdentifier = "fib";
            expect(mod.moduleIdentifier).toBe("fib");
        });
        test("it throws if it's not a string", function () {
            var mod = new llvm.Module("test", context);
            expect(function () { return mod.moduleIdentifier = 2; }).toThrowError("moduleIdentifier needs to be a string");
        });
    });
    describe("sourceFileName", function () {
        test("sets the source file name", function () {
            var mod = new llvm.Module("test", context);
            mod.sourceFileName = "fib.js";
            expect(mod.sourceFileName).toBe("fib.js");
        });
        test("it throws if it's not a string", function () {
            var mod = new llvm.Module("test", context);
            expect(function () { return mod.sourceFileName = 2; }).toThrowError("sourceFileName needs to be a string");
        });
    });
    describe("targetTriple", function () {
        test("sets the target triple", function () {
            var mod = new llvm.Module("test", context);
            mod.targetTriple = "wasm32-unknown-unknown";
            expect(mod.targetTriple).toBe("wasm32-unknown-unknown");
        });
        test("it throws if it's not a string", function () {
            var mod = new llvm.Module("test", context);
            expect(function () { return mod.targetTriple = 1; }).toThrowError("targetTriple needs to be a string");
        });
        it("it throws if it is an illegal target triple", function () {
            var mod = new llvm.Module("test", context);
            mod.targetTriple = "test";
        });
    });
    describe("dump", function () {
        test("dumps the module", function () {
            var mod = new llvm.Module("test", context);
            mod.dump();
        });
    });
    describe("getFunction", function () {
        test("returns the function with the given name", function () {
            var mod = new llvm.Module("test", context);
        });
    });
});
