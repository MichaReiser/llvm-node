import * as llvm from "../../";

describe("PhiNode", () => {
    let context: llvm.LLVMContext;
    let module: llvm.Module;
    let builder: llvm.IRBuilder;

    beforeEach(() => {
        context = new llvm.LLVMContext();
        module = new llvm.Module("test", context);
        builder = new llvm.IRBuilder(context);

        const sin = llvm.Function.create(llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [llvm.Type.getDoubleTy(context)], false), llvm.LinkageTypes.ExternalLinkage, "sin", module);

        builder.setInsertionPoint(llvm.BasicBlock.create(context, "entry", sin));
    });

    describe("addIncoming", () => {
        it("adds an incoming basic block", () => {
            const phi = builder.createPhi(llvm.Type.getDoubleTy(context), 2, "join");
            phi.addIncoming(llvm.ConstantFP.get(context, 2), llvm.BasicBlock.create(context));
        });

        it("throws if the first argument is not a value", () => {
            const phi = builder.createPhi(llvm.Type.getDoubleTy(context), 2, "join");

            expect(() => phi.addIncoming()).toThrowError("addIncome needs to be called with: value: Value, basicBlock: BasicBlock");
            expect(({}) => phi.addIncoming()).toThrowError("addIncome needs to be called with: value: Value, basicBlock: BasicBlock");
        });

        it("throws if the second argument is not a basic block", () => {
            const phi = builder.createPhi(llvm.Type.getDoubleTy(context), 2, "join");
            const value = llvm.ConstantFP.get(context, 2);
            expect(() => phi.addIncoming(value, {})).toThrowError("addIncome needs to be called with: value: Value, basicBlock: BasicBlock");
        });
    });
});