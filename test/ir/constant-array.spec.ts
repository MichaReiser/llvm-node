import * as llvm from "../../";
import {createModule} from "../test-utils";

describe("ir/constant-array", () => {
    let context: llvm.LLVMContext;

    beforeEach(() => {
        ({context} = createModule());
    });

    describe("get", () => {
        it("returns a constant data array", () => {
            const elementType = llvm.Type.getInt32Ty(context);
            const arrayType = llvm.ArrayType.get(elementType, 4);

            const array = llvm.ConstantArray.get(arrayType, [
                llvm.ConstantInt.get(context, 1),
                llvm.ConstantInt.get(context, 2),
                llvm.ConstantInt.get(context, 3),
                llvm.ConstantInt.get(context, 4)
            ]);

            expect(array).toBeInstanceOf(llvm.ConstantDataArray);
            expect(array.type).toEqual(arrayType);
        });
    });
});