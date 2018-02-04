import * as llvm from "../../";
import {createModule} from "../test-utils";

describe("ir/constant-fp", () => {

    let context: llvm.LLVMContext;

    beforeEach(() => {
        ({context} = createModule());
    });

    it("inherits from Constant", () => {
        const value = llvm.ConstantFP.get(context, 10);

        expect(value.isNullValue()).toBe(false);
    });

    it("inherits from Value", () => {
        const value = llvm.ConstantFP.get(context, 10);

        expect(value.name).toBeDefined();
    });

    describe("get", () => {
        it("creates a new float constant", () => {
            const value = llvm.ConstantFP.get(context, 10);

            expect(value.value).toBe(10);
        });

        it("throws if not called with a context as first argument", () => {
            expect(() => llvm.ConstantFP.get()).toThrowError("get needs to be called with: context: LLVMContext, value: number");
            expect(() => llvm.ConstantFP.get({}).toThrowError("get needs to be called with: context: LLVMContext, value: number"));
        });

        it("throws if not called with a number as second argument", () => {
            expect(() => llvm.ConstantFP.get(context, "test")).toThrowError("get needs to be called with: context: LLVMContext, value: number");
        });
    });

    describe("getNaN", () => {
        it("returns the nan value", () => {
            const nan = llvm.ConstantFP.getNaN(llvm.Type.getDoubleTy(context));

            expect(nan).toBeDefined();
        });
    });

    describe("value", () => {
        it("returns the value", () => {
            const value = llvm.ConstantFP.get(context, 10);

            expect(value.value).toBe(10);
        });
    });
});