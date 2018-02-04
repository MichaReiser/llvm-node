import * as llvm from "../..";
import {createModule} from "../test-utils";

describe("ir/array-type", () => {
    let context: llvm.LLVMContext;

    beforeEach(() => {
        ({context} = createModule());
    });

    it("can create a new array type", () => {
        const arrayTy = llvm.ArrayType.get(llvm.Type.getInt32Ty(context), 16);

        expect(arrayTy).toBeInstanceOf(llvm.ArrayType);
        expect(arrayTy.isArrayTy()).toBe(true);
        expect(arrayTy.typeID).toBe(llvm.Type.TypeID.ArrayTyID);
    });

    describe("getNumElements", () => {
       it("returns the number of array elements", () => {
           const arrayTy = llvm.ArrayType.get(llvm.Type.getInt32Ty(context), 16);

           expect(arrayTy.numElements).toBe(16);
       });
    });
});