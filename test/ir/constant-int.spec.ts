import * as llvm from "../../";
import {createModule} from "../test-utils";

describe("ir/constant-int", () => {
    let context: llvm.LLVMContext;

    beforeEach(() => {
        ({context} = createModule());
    });

    describe("get", () => {
       it("returns the int32 value", () => {
          const intValue = llvm.ConstantInt.get(context, 10);

          expect(intValue.value).toBe(10);
          expect(intValue.type).toEqual(llvm.Type.getInt32Ty(context));
       });

      it("returns the int64 value", () => {
        const intValue = llvm.ConstantInt.get(context, '9223372036854775807', 64);

        expect(intValue.toString()).toBe('9223372036854775807');
        expect(intValue.type).toEqual(llvm.Type.getInt64Ty(context));
      });


      it("returns the negative int64 value", () => {
        const intValue = llvm.ConstantInt.get(context, '-9223372036854775807', 64);

        expect(intValue.toString()).toBe('-9223372036854775807');
        expect(intValue.type).toEqual(llvm.Type.getInt64Ty(context));
      });

       it("returns the int value with the specified number of bits", () => {
           const intValue = llvm.ConstantInt.get(context, 10, 32);

           expect(intValue.value).toBe(10);
           expect(intValue.type).toEqual(llvm.Type.getInt32Ty(context));
       });
    });

    describe("getTrue", () => {
        it("returns the true value", () => {
            const trueValue = llvm.ConstantInt.getTrue(context);

            expect(trueValue.type).toEqual(llvm.Type.getInt8Ty(context));
            expect(trueValue.value).toBeDefined();
        });
    });

    describe("getFalse", () => {
        it("returns the false value", () => {
            const trueValue = llvm.ConstantInt.getFalse(context);

            expect(trueValue.type).toEqual(llvm.Type.getInt8Ty(context));
            expect(trueValue.value).toBe(0);
        });
    });
});