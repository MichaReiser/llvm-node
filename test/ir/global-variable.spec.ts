import * as llvm from "../../";

describe("GlobalVariable", () => {
    let context: llvm.LLVMContext;
    let module: llvm.Module;

    beforeEach(() => {
        context = new llvm.LLVMContext();
        module = new llvm.Module("test", context);
    });

    function createGlobalVariable() {
        const intType = llvm.Type.getInt32Ty(context);
        const initializer = llvm.ConstantInt.get(context, 0);
        return new llvm.GlobalVariable(module, intType, true, llvm.LinkageTypes.InternalLinkage, initializer);
    }

    it("can create new instances", () => {
        const intType = llvm.Type.getInt32Ty(context);
        const initializer = llvm.ConstantInt.get(context, 0);
        const globalVariable = new llvm.GlobalVariable(module, intType, true, llvm.LinkageTypes.InternalLinkage, initializer);

        expect(globalVariable).toBeInstanceOf(llvm.GlobalVariable);
        expect(globalVariable.type).toEqual(intType.getPointerTo());
        expect(globalVariable.constant).toBe(true);
        expect(globalVariable.initializer).toEqual(initializer);
    });

    it("inherits from value", () => {
       const globalVariable = createGlobalVariable();
       globalVariable.name = "test";

       expect(globalVariable.hasName()).toBe(true);
    });

    describe("constant", () => {
        it("returns true for a constant initializer", () => {
            const globalVariable = createGlobalVariable();

            expect(globalVariable.constant).toBe(true);
        });

        it("changes the constant value", () => {
            const globalVariable = createGlobalVariable();
            globalVariable.constant = false;

            expect(globalVariable.constant).toBe(false);
        })
    });

    describe("initializer", () => {
        it("returns the initializer", () => {
            const intType = llvm.Type.getInt32Ty(context);
            const initializer = llvm.ConstantInt.get(context, 0);
            const globalVariable = new llvm.GlobalVariable(module, intType, true, llvm.LinkageTypes.InternalLinkage, initializer);

            expect(globalVariable.initializer).toEqual(initializer);
        });

        it("changes the initializer", () => {
            const globalVariable = createGlobalVariable();
            const newInitializer = llvm.ConstantInt.get(context, 10);

            globalVariable.initializer = newInitializer;
            expect(globalVariable.initializer).toEqual(newInitializer);
        });
    });

    describe("hasGlobalUnnamedAddr", () => {
        it("is false by default", () => {
            const globalVariable = createGlobalVariable();

            expect(globalVariable.hasGlobalUnnamedAddr()).toBe(false);
        });

        it("can be changed with setUnnamedAddr", () => {
            const globalVariable = createGlobalVariable();

            globalVariable.setUnnamedAddr(llvm.UnnamedAddr.Global);

            expect(globalVariable.hasGlobalUnnamedAddr()).toBe(true);
        });
    });
});