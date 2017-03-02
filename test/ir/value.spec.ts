const llvm = require("../../index");

describe("Value", () => {
    const context: llvm.LLVMContext;

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

            expect(() => value.name = 1000).toThrowError("String required");
        });
    });

    describe("type", () => {
        it("returns the type of the value", () => {
            const doubleTy = llvm.Type.getDoubleTy(context);
            const value = new llvm.Argument(doubleTy);

            expect(value.type).toEqual(doubleTy);
        });
    });

    describe("dump", () => {
        it("dumps the value", () => {
            const value = new llvm.Argument(llvm.Type.getDoubleTy(context));
            value.dump();
        });
    });
});