import * as llvm from "../../";
import {createModule} from "../test-utils";

describe("ir/constant-data-array", () => {
    let context: llvm.LLVMContext;

    beforeEach(() => {
        ({context} = createModule());
    });

    describe("get", () => {
        it("creates a uint32 array", () => {
            const array = llvm.ConstantDataArray.get(context, new Uint32Array([1, 2, 3, 4]));

            expect(array).toBeInstanceOf(llvm.ConstantDataArray);
            expect(array.type.isArrayTy()).toBe(true);
            expect(array.type).toEqual(llvm.ArrayType.get(llvm.Type.getInt32Ty(context), 4));
        });

        it("creates a float array", () => {
            const array = llvm.ConstantDataArray.get(context, new Float32Array([1.0, 2, 3, 4]));

            expect(array).toBeInstanceOf(llvm.ConstantDataArray);
            expect(array.type).toEqual(llvm.ArrayType.get(llvm.Type.getFloatTy(context), 4));
        });

        it("creates a float64 array", () => {
            const array = llvm.ConstantDataArray.get(context, new Float64Array([1, 2, 3, 4]));

            expect(array).toBeInstanceOf(llvm.ConstantDataArray);
            expect(array.type).toEqual(llvm.ArrayType.get(llvm.Type.getDoubleTy(context), 4));
        });
    });
});