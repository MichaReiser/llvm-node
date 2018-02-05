import * as llvm from "../../";
import {createBuilder} from "../test-utils";

describe("ir/call-inst", () => {
    let context: llvm.LLVMContext;
    let module: llvm.Module;
    let builder: llvm.IRBuilder;

    beforeEach(() => {
        ({builder, context, module} = createBuilder());
    });

    function createCall() {
        const doubleTy = llvm.Type.getDoubleTy(context);
        const sinType = llvm.FunctionType.get(doubleTy, [doubleTy], false);
        const sin = llvm.Function.create(sinType, llvm.LinkageTypes.ExternalWeakLinkage, "sin", module);

        const mainType = llvm.FunctionType.get(llvm.Type.getInt32Ty(context), false);
        const fn = llvm.Function.create(mainType, llvm.LinkageTypes.ExternalWeakLinkage, "main", module);
        const block = llvm.BasicBlock.create(context, "entry", fn);
        builder.setInsertionPoint(block);

        const call = builder.createCall(sin, [llvm.ConstantFP.get(context, 12.0)], "sinResult");

        return {call, fn, sin};
    }

    describe("callingConv", () => {
        it("sets and gets the calling convention", () => {
            const {call} = createCall();

            call.callingConv = llvm.CallingConv.Fast;

            expect(call.callingConv).toEqual(llvm.CallingConv.Fast);
        });
    });

    describe("getNumArgOperands", () => {

        it("returns the number of operands", () => {
            const {call} = createCall();

            expect(call.getNumArgOperands()).toBe(1);
        });
    });

    describe("attributes", () => {
        it("adds the dereferenceable attribute to the return value if called with index 0", () => {
            const {call} = createCall();

            call.addDereferenceableAttr(0, 16);

            expect(call.hasRetAttr(llvm.Attribute.AttrKind.Dereferenceable)).toBe(true);
        });

        it("adds the dereferenceable attribute to the first parameter if called with index 1", () => {
            const {call} = createCall();

            call.addDereferenceableAttr(1, 16);

            expect(call.paramHasAttr(0, llvm.Attribute.AttrKind.Dereferenceable)).toBe(true);
        });

        it("returns false if the return value or parameter has not the requested attribute", () => {
            const {call} = createCall();

            expect(call.hasRetAttr(llvm.Attribute.AttrKind.Dereferenceable)).toBe(false);
            expect(call.paramHasAttr(0, llvm.Attribute.AttrKind.Dereferenceable)).toBe(false);
        });
    });
});