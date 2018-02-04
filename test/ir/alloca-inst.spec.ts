import * as llvm from "../../";
import {createBuilderWithBlock} from "../test-utils";

describe("ir/alloca-inst", () => {

    let builder: llvm.IRBuilder;
    let context: llvm.LLVMContext;

    beforeEach(() => {
        const builderWithContext = createBuilderWithBlock();
        context = builderWithContext.context;
        builder = builderWithContext.builder;
    });

    function allocateInt32() {
        const int32Type = llvm.Type.getInt32Ty(context);
        return builder.createAlloca(int32Type, undefined, "test");
    }

    describe("allocatedType", () => {
        it("returns the type of the allocation", () => {
            const allocation = allocateInt32();

            expect(allocation.allocatedType).toEqual(llvm.Type.getInt32Ty(context));
        });

        it("can change the type of the allocation", () => {
            const allocation = allocateInt32();
            const doubleTy = llvm.Type.getDoubleTy(context);

            allocation.allocatedType = doubleTy;

            expect(allocation.allocatedType).toEqual(doubleTy);
        });
    });

    describe("alignment", () => {
        it("sets and gets the alignment", () => {
            const allocation = allocateInt32();

            allocation.alignment = 4;
            expect(allocation.alignment).toBe(4);
        });
    });

    describe("type", () => {
        it("returns the llvm type of the allocation", () => {
            const allocation = allocateInt32();

            expect(allocation.type).toEqual(llvm.Type.getInt32Ty(context).getPointerTo());
        });
    });
});