import * as llvm from "../../";

describe("GlobalVariable", () => {
    let context: llvm.LLVMContext;
    let module: llvm.Module;

    beforeEach(() => {
        context = new llvm.LLVMContext();
        module = new llvm.Module("test", context);
    });

    test("Constructor returns a new instance", () => {
        const intType = llvm.Type.getInt32Ty(context);
        const initializer = llvm.ConstantInt.get(context, 0);
        const globalVariable = new llvm.GlobalVariable(module, intType, true, llvm.LinkageTypes.InternalLinkage, initializer);

        expect(globalVariable).toBeInstanceOf(llvm.GlobalVariable);
        expect(globalVariable.type).toEqual(intType.getPointerTo());
        expect(globalVariable.constant).toBe(true);
        expect(globalVariable.initializer).toEqual(initializer);
    });
});