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

    module Attribute {
        enum AttrKind {
            Alignment,
            AllocSize,
            AlwaysInline,
            ArgMemOnly,
            Builtin,
            ByVal,
            Cold,
            Convergent,
            Dereferenceable,
            DereferenceableOrNull,
            InAlloca,
            InReg,
            InaccessibleMemOnly,
            InaccessibleMemOrArgMemOnly,
            InlineHint,
            JumpTable,
            MinSize,
            Naked,
            Nest,
            NoAlias,
            NoBuiltin,
            NoCapture,
            NoDuplicate,
            NoImplicitFloat,
            NoInline,
            NoRecurse,
            NoRedZone,
            NoReturn,
            NoUnwind,
            NonLazyBind,
            NonNull,
            OptimizeForSize,
            OptimizeNone,
            ReadNone,
            ReadOnly,
            Returned,
            ReturnsTwice,
            SExt,
            SafeStack,
            SanitizeAddress,
            SanitizeMemory,
            SanitizeThread,
            StackAlignment,
            StackProtect,
            StackProtectReq,
            StackProtectStrong,
            StructRet,
            SwiftError,
            SwiftSelf,
            UWTable,
            WriteOnly,
            ZExt
        }
    }

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

    enum VisibilityTypes {
        Hidden,
        Default,
        Protected
    }

    class Value {
        static MaxAlignmentExponent: number;
        static MaximumAlignment: number;
        protected constructor();

        type: Type;
        name: string;

        dump?: () => void;
        hasName(): boolean;

        /**
         * Deletes the value. It is, therefore, forbidden to use the value any further
         */
        release(): void;
        deleteValue(): void;

        replaceAllUsesWith(value: Value): void;
        useEmpty(): boolean;

    }

    class Argument extends Value {
        argumentNumber: number;
        parent?: Function;

        constructor(type: Type, name?: string, fn?: Function, argNo?: number);

        addAttr(kind: Attribute.AttrKind): void;
        addDereferenceableAttr(bytes: number): void;
    }

    class AllocaInst extends Value {
        alignment: number;
        type: PointerType;
        allocatedType: Type;

        private constructor();
    }

    class BasicBlock extends Value {
        static create(context: LLVMContext, name?: string, parent?: Function, insertBefore?: BasicBlock): BasicBlock;
        private constructor();

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
        static getNaN(type: Type): Constant;

        private constructor();

        value: number;
    }

    class ConstantInt extends Constant {
        static get(context: LLVMContext, value: number, numBits?: number, signed?: boolean): ConstantInt;
        static getFalse(context: LLVMContext): ConstantInt;
        static getTrue(context: LLVMContext): ConstantInt;

        private constructor();

        value: number;
    }

    class ConstantPointerNull extends Constant {
        static get(pointerType: PointerType): ConstantPointerNull;

        private constructor();
    }

    class ConstantArray extends Constant {
        static get(arrayType: ArrayType, elements: Constant[]): Constant;
    }

    class ConstantDataArray extends Constant {
        static get(llvmContext: LLVMContext, values: number[]): Constant;
        static getString(llvmContext: LLVMContext, value: string): Constant;

        private constructor();
    }

    class ConstantStruct extends Constant {
        static get(structType: StructType, values: Constant[]): Constant;
        private constructor();
    }

    class Function extends Constant {
        static create(functionType: FunctionType, linkageTypes: LinkageTypes, name?: string, module?: Module): Function;

        callingConv: CallingConv;
        visibility: VisibilityTypes;
        type: PointerType & { elementType: FunctionType };

        private constructor();
        addBasicBlock(basicBlock: BasicBlock): void;
        addDereferenceableAttr(attributeIndex: number, bytes: number): void;
        addDereferenceableOrNullAttr(attributeIndex: number, bytes: number): void;
        addFnAttr(attribute: Attribute.AttrKind): void;
        getArguments(): Argument[];
        getEntryBlock(): BasicBlock | null;
        viewCFG(): void;
    }

    class GlobalVariable extends Constant {
        constant: boolean;
        initializer: Constant | undefined;

        setUnnamedAddr(unnamedAddr: UnnamedAddr): void;
        hasGlobalUnnamedAddr(): boolean;

        constructor(module: Module, type: Type, constant: boolean, linkageType: LinkageTypes, initializer?: Constant, name?: string);
    }

    enum UnnamedAddr {
        None,
        Local,
        Global
    }

    class PhiNode extends Value {
        private constructor();

        addIncoming(value: Value, basicBlock: BasicBlock): void;
    }


    class CallInst extends Value {
        callingConv: CallingConv;

        private constructor();

        addDereferenceableAttr(index: number, size: number): void;
        getNumArgOperands(): number;
    }

    enum CallingConv {
        C,
        Fast,
        Cold,
        GHC,
        HiPE,
        WebKit_JS,
        AnyReg,
        PreserveMost,
        PreserveAll,
        CXX_FAST_TLS,
        FirstTargetCC,
        X86_StdCall,
        X86_FastCall,
        ARM_APCS,
        ARM_AAPCS,
        ARM_AAPCS_VFP,
        MSP430_INTR,
        X86_ThisCall,
        PTX_Kernel,
        PTX_Device,
        SPIR_FUNC,
        SPIR_KERNEL,
        Intel_OCL_BI,
        X86_64_SysV,
        X86_64_Win64,
        X86_VectorCall,
        HHVM,
        HHVM_C,
        X86_INTR,
        AVR_INTR,
        AVR_SIGNAL,
        AVR_BUILTIN,
        MaxID,
    }

    class UndefValue {
        private constructor();

        static get(type: Type): UndefValue;
    }

    class DataLayout {
        constructor(layout: string);
        getStringRepresentation(): string;
        getPointerSize(as: number): number;
        getPrefTypeAlignment(type: Type): number;
        getTypeStoreSize(type: Type): number;
        getIntPtrType(context: LLVMContext, as: number): Type;
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

        static getFloatTy(context: LLVMContext): Type;
        static getDoubleTy(context: LLVMContext): Type;
        static getVoidTy(context: LLVMContext): Type;
        static getLabelTy(context: LLVMContext): Type;
        static getInt1Ty(context: LLVMContext): Type;
        static getInt8Ty(context: LLVMContext): Type;
        static getInt16Ty(context: LLVMContext): Type;
        static getInt32Ty(context: LLVMContext): Type;
        static getInt64Ty(context: LLVMContext): Type;
        static getInt128Ty(context: LLVMContext): Type;
        static getIntNTy(context: LLVMContext, N: number): Type;

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
        getPrimitiveSizeInBits(): number;

        toString(): string;
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

        elementType: Type;

        private constructor();
    }

    class ArrayType extends Type {
        static get(elementType: Type, numElements: number): ArrayType;

        private constructor();
    }

    class StructType extends Type {
        static create(context: LLVMContext, name?: string): StructType;
        static get(context: LLVMContext, elements: Type[], isPacked?: boolean): StructType;

        name: string;
        readonly numElements: number;

        private constructor();

        getElementType(index: number): Type;
        setBody(elements: Type[], packed?: boolean): void;
    }

    class IRBuilder {
        constructor(context: LLVMContext);
        constructor(basicBlock: BasicBlock, beforeInstruction: Value);

        setInsertionPoint(basicBlock: BasicBlock): void;
        createAdd(lhs: Value, rhs: Value, name?: string): Value;
        createAlloca(type: Type, arraySize?: Value, name?: string): AllocaInst;
        createAlignedLoad(ptr: Value, align: number, name?: string): Value;
        createAlignedStore(value: Value, ptr: Value, align: number, isVolatile?: boolean): Value;
        createAnd(lhs: Value, rhs: Value, name?: string): Value;
        createAShr(lhs: Value, rhs: Value, name?: string): Value;
        createBitCast(value: Value, destType: Type, name?: string): Value;
        createBr(basicBlock: BasicBlock): Value;
        createCall(callee: Value, args: Value[], name?: string): CallInst;
        createCondBr(condition: Value, then: BasicBlock, elseBlock: BasicBlock): Value;
        createExtractValue(agg: Value, idxs: number[], name?: string): Value;
        createFAdd(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpOGE(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpOGT(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpOLE(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpOLT(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpOEQ(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpONE(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpUGE(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpUGT(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpULE(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpULT(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpUEQ(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpUNE(lhs: Value, rhs: Value, name?: string): Value;
        createFDiv(lhs: Value, rhs: Value, name?: string): Value;
        createFNeg(value: Value, name?: string): Value;
        createFMul(lhs: Value, rhs: Value, name?: string): Value;
        createFRem(lhs: Value, rhs: Value, name?: string): Value;
        createFSub(lhs: Value, rhs: Value, name?: string): Value;
        createFPToSI(value: Value, type: Type, name?: string): Value;
        createGlobalString(str: string, name?: string, addressSpace?: number): Value;
        createGlobalStringPtr(str: string, name?: string, addressSpace?: number): Value;
        createInBoundsGEP(ptr: Value, idxList: Value[], name?: string): Value;
        createInBoundsGEP(type: Type, ptr: Value, idxList: Value[], name?: string): Value;
        createInsertValue(agg: Value, val: Value, idxList: number, name?: string): Value;
        createIntCast(vlaue: Value, type: Type, isSigned: boolean, name?: string): Value;
        createICmpEQ(lhs: Value, rhs: Value, name?: string): Value;
        createICmpNE(lhs: Value, rhs: Value, name?: string): Value;
        createICmpSGE(lhs: Value, rhs: Value, name?: string): Value;
        createICmpSGT(lhs: Value, rhs: Value, name?: string): Value;
        createICmpSLE(lhs: Value, rhs: Value, name?: string): Value;
        createICmpSLT(lhs: Value, rhs: Value, name?: string): Value;
        createICmpULT(lhs: Value, rhs: Value, name?: string): Value;
        createLoad(ptr: Value, name?: string): Value;
        createLShr(lhs: Value, rhs: Value, name?: string): Value;
        createMul(lhs: Value, rhs: Value, name?: string): Value;
        createNeg(value: Value, name?: string, hasNUW?: boolean, hasNSW?: boolean): Value;
        createNot(value: Value, name?: string): Value;
        createOr(lhs: Value, rhs: Value, name?: string): Value;
        createXor(lhs: Value, rhs: Value, name?: string): Value;
        createPhi(type: Type, numReservedValues: number, name?: string): PhiNode;
        createPtrToInt(value: Value, destType: Type, name?: string): Value;
        createRet(value: Value): Value;
        createRetVoid(): Value;
        createSelect(condition: Value, trueValue: Value, falseValue: Value, name?: string): Value;
        createSDiv(lhs: Value, rhs: Value, name?: string): Value;
        createShl(lhs: Value, rhs: Value, name?: string): Value;
        createSIToFP(value: Value, type: Type, name?: string): Value;
        createUIToFP(value: Value, type: Type, name?: string): Value;
        createSub(lhs: Value, rhs: Value, name?: string): Value;
        createStore(value: Value, ptr: Value, isVolatile?: boolean): Value;
        createSRem(lhs: Value, rhs: Value, name?: string): Value;
        createZExt(value: Value, destType: Type, name?: string): Value;
        getInsertBlock(): BasicBlock;
    }

    class LLVMContext {
        constructor();
    }

    class Module {
        empty: boolean;
        moduleIdentifier: string;
        sourceFileName: string;
        dataLayout: DataLayout;
        targetTriple: string;

        constructor(moduleId: string, context: LLVMContext);

        dump?: () => void;
        print(): string;
        getFunction(name: string): Function;
        getOrInsertFunction(name: string, functionType: FunctionType): Constant;
        getGlobalVariable(name: string, allowInternal?: boolean): GlobalVariable;
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
