const llvm = require("../../");

describe("Constant", () => {
    let context: llvm.LLVMContext;
    let module: llvm.Module;

    beforeEach(() => {
        context = new llvm.LLVMContext();
        module = new llvm.Module("test", context);
    });

    it("inherits from value", () => {
        expect(llvm.Constant.getNullValue(llvm.Type.getDoubleTy(context)).name).toBeDefined();
    });

    describe("getNullValue", () => {
        it("returns 0 double value for double type", () => {
            expect(llvm.Constant.getNullValue(llvm.Type.getDoubleTy(context)).value).toBe(0);
        });

        it("throws if not a type is passed", () => {
            expect(() => llvm.Constant.getNullValue(12)).toThrowError("getNullValue needs to be called with: type: Type");
        });
    });

    describe("isNullValue", () => {
        it("returns true for a null value", () => {
            const nullValue = llvm.Constant.getNullValue(llvm.Type.getDoubleTy(context));
            expect(nullValue.isNullValue()).toBe(true);
        });

        it("returns false otherwise", () => {
            const nullValue = llvm.Constant.getAllOnesValue(llvm.Type.getDoubleTy(context));
            expect(nullValue.isNullValue()).toBe(false);
        });

        it("throws if not called with a type", () => {
             expect(() => llvm.Constant.getNullValue({})).toThrowError("getNullValue needs to be called with: type: Type");
        });
    });

    describe("isNullValue", () => {
        it("returns true for a all one value", () => {
            const nullValue = llvm.Constant.getAllOnesValue(llvm.Type.getDoubleTy(context));
            expect(nullValue.isAllOnesValue()).toBe(true);
        });

        it("returns false otherwise", () => {
            const nullValue = llvm.Constant.getNullValue(llvm.Type.getDoubleTy(context));
            expect(nullValue.isAllOnesValue()).toBe(false);
        });

        it("throws if not called with a type", () => {
            expect(() => llvm.Constant.getAllOnesValue({})).toThrowError("getAllOnesValue needs to be called with: type: Type");
        });
    });
});