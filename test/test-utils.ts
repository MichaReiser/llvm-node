import * as llvm from "..";

export function createModule({moduleId = "test"} = {}) {
    const context = new llvm.LLVMContext();
    return {
        context,
        module: new llvm.Module(moduleId, context)
    };
}

export function createBuilder({moduleId = "test"} = {}) {
    const moduleAndContext = createModule({moduleId});
    return {
        ...moduleAndContext,
        builder: new llvm.IRBuilder(moduleAndContext.context)
    };
}

export function createBuilderWithBlock({moduleId = "test", blockName = "entry", fnName = "test"} = {}) {
    const {builder, module, context} = createBuilder({moduleId});

    const fn = llvm.Function.create(llvm.FunctionType.get(llvm.Type.getVoidTy(context), false), llvm.LinkageTypes.ExternalWeakLinkage, fnName, module);
    const block = llvm.BasicBlock.create(context, blockName, fn);
    builder.setInsertionPoint(block);

    return {
        builder, module, context, basicBlock: block, fn
    };
}