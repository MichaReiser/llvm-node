import * as llvm from "../../";
import {createModule} from "../test-utils";

describe("ir/constant-struct", () => {
    let context: llvm.LLVMContext;

    beforeEach(() => {
        ({context} = createModule());
    });

    describe("get", () => {
        it("creates a constant struct", () => {
            const structType = llvm.StructType.get(context, [
                llvm.Type.getInt32Ty(context),
                llvm.Type.getInt64Ty(context)
            ]);

            const struct = llvm.ConstantStruct.get(structType, [llvm.ConstantInt.get(context, 1), llvm.ConstantInt.get(context, 2, 64)]);

            expect(struct).toBeInstanceOf(llvm.ConstantStruct);
            expect(struct.type.equals(structType)).toBe(true);
            expect(struct.isNullValue()).toBe(false);
        });
    });
});