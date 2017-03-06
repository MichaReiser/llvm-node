import * as llvm from "../../";

describe("Verifier", () => {
    let context: llvm.LLVMContext;
    let mod: llvm.Module;

    beforeEach(() => {
            context = new llvm.LLVMContext();
            mod = new llvm.Module("test", context);
    });

    describe("verifyModule", () => {
        it("returns if the module is valid", () => {
            const sin = llvm.Function.create(llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [], false), llvm.LinkageTypes.ExternalLinkage, "sin", mod);
            const entry = llvm.BasicBlock.create(context, "entry", sin);

            const builder = new llvm.IRBuilder(context);
            builder.setInsertionPoint(entry);
            builder.createRet(llvm.ConstantFP.get(context, 10));

            llvm.verifyModule(mod);
        });

        it("throws if the module is invalid", () => {
            const sin = llvm.Function.create(llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [], false), llvm.LinkageTypes.ExternalLinkage, "sin", mod);
            const entry = llvm.BasicBlock.create(context, "entry", sin);

            const builder = new llvm.IRBuilder(context);
            builder.setInsertionPoint(entry);

            expect(() => llvm.verifyModule(mod)).toThrowError("Module verification failed: Basic Block in function 'sin' does not have terminator!");
        });
    });

    describe("verifyFunction", () => {
        it("returns if the function is valid", () => {
            const sin = llvm.Function.create(llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [], false), llvm.LinkageTypes.ExternalLinkage, "sin", mod);
            const entry = llvm.BasicBlock.create(context, "entry", sin);

            const builder = new llvm.IRBuilder(context);
            builder.setInsertionPoint(entry);
            builder.createRet(llvm.ConstantFP.get(context, 10));

            llvm.verifyFunction(sin);
        });

        it("throws if the function is invalid", () => {
            const sin = llvm.Function.create(llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [], false), llvm.LinkageTypes.ExternalLinkage, "sin", mod);
            const entry = llvm.BasicBlock.create(context, "entry", sin);

            const builder = new llvm.IRBuilder(context);
            builder.setInsertionPoint(entry);

            expect(() => llvm.verifyFunction(sin)).toThrowError("Function verification failed: Basic Block in function 'sin' does not have terminator!");
        });
    });
});