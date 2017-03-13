declare namespace llvm {

    // bitcode/bitcodewriter
    function writeBitcodeToFile(module: Module, filename: string): void;

    // IR/verifier
    function verifyModule(mod: Module): void;

    function verifyFunction(fun: Function): void;

    // support
    function initializeAllTargetInfos(): void;

    function initializeAllTargets(): void;

    function initializeAllTargetMCs(): void;

    function initializeAllAsmParsers(): void;

    function initializeAllAsmPrinters(): void;

    //ir
    enum LinkageTypes {
        ExternalLinkage,
        AvailableExternallyLinkage,
        LinkOnceAnyLinkage,
        LinkOnceODRLinkage,
        WeakAnyLinkage,
        WeakODRLinkage,
        AppendingLinkage,
        InternalLinkage,
        PrivateLinkage,
        ExternalWeakLinkage,
        CommonLinkage
    }

    class Value {
        static MaxAlignmentExponent: number;
        static MaximumAlignment: number;
        protected constructor();

        type: Type;
        name: string;

        dump(): void;
        hasName(): boolean;

        /**
         * Deletes the value. It is, therefore, forbidden to use the value any further
         */
        release(): void;

        replaceAllUsesWith(value: Value): void;
        useEmpty(): boolean;
    }

    interface Argument extends Value {
        argumentNumber: number;
        parent?: Function;

        constructor(type: Type, name?: string, func?: Function);
    }

    interface AllocaInst extends Value {
        allocatedType: Type;
    }

    class BasicBlock extends Value {
        static create(context: LLVMContext, name?: string, parent?: Function, insertBefore?: BasicBlock): BasicBlock;
        protected constructor();

        parent?: Function;
        empty: boolean;
        eraseFromParent(): void;
        getTerminator(): Value | undefined;
    }

    class Constant extends Value {
        static getNullValue(type: Type): Constant;

        static getAllOnesValue(type: Type): Constant;

        protected constructor();

        isNullValue(): boolean;

        isOneValue(): boolean;

        isAllOnesValue(): boolean;
    }

    class ConstantFP extends Constant {
        static get(context: LLVMContext, value: number): ConstantFP;

        private constructor();

        value: number;
    }

    class ConstantInt extends Constant {
        static get(context: LLVMContext, value: number): ConstantInt;
        static getFalse(context: LLVMContext): ConstantInt;
        static getTrue(context: LLVMContext): ConstantInt;

        private constructor();

        value: number;
    }

    class ConstantPointerNull extends Constant {
        static get(pointerType: PointerType): ConstantPointerNull;

        private constructor();
    }

    interface PhiNode extends Value {
        addIncoming(value: Value, basicBlock: BasicBlock): void;
    }

    class Function extends Constant {
        static create(functionType: FunctionType, linkageTypes: LinkageTypes, name?: string, module?: Module): Function;

        private constructor();
        addBasicBlock(basicBlock: BasicBlock);
        getArguments(): Argument[];
        getEntryBlock(): BasicBlock;
    }

    interface DataLayout {
        constructor(layout: string);

        getStringRepresentation(): string;
    }

    class Type {
        static TypeID: {
            VoidTyID: number,
            HalfTyID: number,
            FloatTyID: number,
            DoubleTyID: number,
            X86_FP80TyID: number,
            PPC_FP128TyID: number,
            LabelTyID: number,
            MetadataTyID: number,
            X86_MMXTyID: number,
            TokenTyID: number,
            IntegerTyID: number,
            FunctionTyID: number,
            StructTyID: number,
            ArrayTyID: number,
            PointerTyID: number,
            VectorTyID: number
        };

        static getDoubleTy(context: LLVMContext): Type;
        static getVoidTy(context: LLVMContext): Type;
        static getLabelTy(context: LLVMContext): Type;
        static getInt1Ty(context: LLVMContext): Type;
        static getInt8Ty(context: LLVMContext): Type;
        static getInt16Ty(context: LLVMContext): Type;
        static getInt32Ty(context: LLVMContext): Type;
        static getInt64Ty(context: LLVMContext): Type;
        static getInt128Ty(context: LLVMContext): Type;

        static getInt1PtrTy(context: LLVMContext, AS?: number): PointerType;
        static getInt8PtrTy(context: LLVMContext, AS?: number): PointerType;
        static getInt32PtrTy(context: LLVMContext, AS?: number): PointerType;

        protected constructor();

        isVoidTy(): boolean;
        isFloatTy(): boolean;
        isDoubleTy(): boolean;
        isLabelTy(): boolean;
        isIntegerTy(): boolean;
        isFunctionTy(): boolean;
        isStructTy(): boolean;
        isArrayTy(): boolean;
        isPointerTy(): boolean;
        isDoubleTy(): boolean;
        isVoidTy(): boolean;
        getPointerTo(addressSpace?: number): PointerType;
    }

    class FunctionType extends Type {
        static get(result: Type, isVarArg: boolean): FunctionType;
        static get(result: Type, params: Type[], isVarArg: boolean): FunctionType;

        static isValidReturnType(type: Type): boolean;

        static isValidArgumentType(type: Type): boolean;

        returnType: Type;
        isVarArg: boolean;
        numParams: number;

        private constructor();
        getParams(): Type[];
        getParamType(index: number): Type;
    }

    class PointerType extends Type {
        static get(elementType: Type, AS: number): PointerType;

        private constructor();
    }

    class ArrayType extends Type {
        static get(elementType: Type, numElements: number): ArrayType;

        private constructor();
    }

    interface IRBuilder {
        constructor(context: LLVMContext);
        constructor(basicBlock: BasicBlock);

        setInsertionPoint(basicBlock: BasicBlock): void;
        createAdd(lhs: Value, rhs: Value, name?: string): Value;
        createAlloca(type: Type, arraySize?: Value, name?: string): AllocaInst;
        createBr(basicBlock: BasicBlock): Value;
        createCall(callee: Function, args: Value[], name?: string): Value;
        createCondBr(condition: Value, then: BasicBlock, elseBlock: BasicBlock): Value;
        createFAdd(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpOGT(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpOLE(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpOLT(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpOEQ(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpULE(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpULT(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpUEQ(lhs: Value, rhs: Value, name?: string): Value;
        createFDiv(lhs: Value, rhs: Value, name?: string): Value;
        createFRem(lhs: Value, rhs: Value, name?: string): Value;
        createFSub(lhs: Value, rhs: Value, name?: string): Value;
        createFPToSI(value: Value, type: Type, name?: string): Value;
        createInBoundsGEP(ptr: Value, idxList: Value[], name?: string): Value;
        createICmpEQ(lhs: Value, rhs: Value, name?: string): Value;
        createICmpSGT(lhs: Value, rhs: Value, name?: string): Value;
        createICmpSLE(lhs: Value, rhs: Value, name?: string): Value;
        createICmpSLT(lhs: Value, rhs: Value, name?: string): Value;
        createLoad(ptr: Value, name?: string): Value;
        createMul(lhs: Value, rhs: Value, name?: string): Value;
        createPhi(type: Type, numReservedValues: number, name?: string): PhiNode;
        createRet(value: Value): Value;
        createRetVoid(): Value;
        createSDiv(lhs: Value, rhs: Value, name?: string): Value;
        createSIToFP(value: Value, type: Type, name?: string): Value;
        createSub(lhs: Value, rhs: Value, name?: string): Value;
        createStore(value: Value, ptr: Value, isVolatile?: boolean): Value;
        createSRem(lhs: Value, rhs: Value, name?: string): Value;
        getInsertBlock(): BasicBlock;
    }

    interface LLVMContext {
        constructor();
    }

    interface Module {
        empty: boolean;
        moduleIdentifier: string;
        sourceFileName: string;
        targetTriple: string;

        constructor(moduleId: string, context: LLVMContext);

        dump(): void;

        getFunction(name: string): Function;

        setDataLayout(dataLayout: DataLayout): void;
    }

    // support
    class TargetRegistry {
        private constructor();
        static lookupTarget(target: string): Target;
    }

    class Target {
        name: string;
        shortDescription: string;
        private constructor();
        createTargetMachine(triple: string, cpu: string): TargetMachine;
    }

    // target
    interface TargetMachine {
        createDataLayout(): DataLayout;
    }
}

export = llvm;
export as namespace llvm;
