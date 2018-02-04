import * as llvm from "../../";
import {createModule} from "../test-utils";

describe("ir/constant-pointer-null", () => {
    let context: llvm.LLVMContext;

    beforeEach(() => {
        ({context} = createModule());
    });

    describe("get", () => {
        it("creates a null pointer", () => {
           const nullPointer = llvm.ConstantPointerNull.get(llvm.Type.getInt32Ty(context).getPointerTo());

           expect(nullPointer).toBeDefined();
           expect(nullPointer).toBeInstanceOf(llvm.ConstantPointerNull);
           expect(nullPointer.type.equals(llvm.Type.getInt32Ty(context).getPointerTo())).toBe(true);
           expect(nullPointer.isNullValue()).toBe(true);
        });
    });
});