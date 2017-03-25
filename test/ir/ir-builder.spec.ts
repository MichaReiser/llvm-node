import * as llvm from "../../";

describe("IRBuilder", () => {
    let context: llvm.LLVMContext;
    let module: llvm.Module;
    let builder: llvm.IRBuilder;

    beforeEach(() => {
        context = new llvm.LLVMContext();
        module = new llvm.Module("test", context);
        builder = new llvm.IRBuilder(context);
    });

    // getGlobalStringPtr requires a function and entry block
    test("getGlobalStringPtr returns a ptr to a string", () => {
        const fnType = llvm.FunctionType.get(llvm.Type.getVoidTy(context), [], false);
        const fn = llvm.Function.create(fnType, llvm.LinkageTypes.ExternalWeakLinkage, "test", module);
        const entry = llvm.BasicBlock.create(context, "entry", fn);

        builder.setInsertionPoint(entry);

        const ptr = builder.createGlobalStringPtr("Hello world", "test", 0);

        expect(ptr).toBeDefined();
        expect(ptr.type.isPointerTy()).toBe(true);
    });

    test("getGlobalString returns a ptr to a string array", () => {
        const fnType = llvm.FunctionType.get(llvm.Type.getVoidTy(context), [], false);
        const fn = llvm.Function.create(fnType, llvm.LinkageTypes.ExternalWeakLinkage, "test", module);
        const entry = llvm.BasicBlock.create(context, "entry", fn);

        builder.setInsertionPoint(entry);

        const arrayPtr = builder.createGlobalString("Hello world", "test", 0);

        expect(arrayPtr).toBeDefined();
    });
});