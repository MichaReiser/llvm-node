declare module "llvm-node" {
    // bitcode/bitcodewriter
    export function writeBitcodeToFile(module: Module, filename: string): void;

    // IR/verifier
    export function verifyModule(mod: Module): void;
    export function verifyFunction(fun: Function): void;

    // support
    export function initializeAllTargetInfos(): void;
    export function initializeAllTargets(): void;
    export function initializeAllTargetMCs(): void;
    export function initializeAllAsmParsers(): void;
    export function initializeAllAsmPrinters(): void;

    //ir
    export enum LinkageTypes {
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

    export class Value {
        static MaxAlignmentExponent: number;
        static MaximumAlignment: number;

        type: Type;
        name: string;

        dump(): void;
        hasName(): boolean;

        /**
         * Deletes the value. It is, therefore, forbidden to use the value any further
         */
        release(): void;
    }

    export class Argument extends Value {
        argumentNumber: number;
        parent: Function;
        constructor(type: Type, name?: string, func?: Function);
    }

    export class BasicBlock extends Value {
        static create(context: LLVMContext, name?: string, parent?: Function, insertBefore?: BasicBlock): BasicBlock;

        parent: Function;
    }

    export class Constant extends Value {
        static getNullValue(type: Type): Constant;
        static getAllOnesValue(type: Type): Constant;

        isNullValue(): boolean;
        isOneValue(): boolean;
        isAllOnesValue(): boolean;
    }

    export class ConstantFP extends Constant {
        static get(context: LLVMContext, value: number): ConstantFP;

        value: number;
    }

    export class PhiNode {
        addIncoming(value: Value, basicBlock: BasicBlock): void;
    }

    export class Function extends Constant {
        static create(functionType: FunctionType, linkageTypes: LinkageTypes, name?: string, module?: Module): Function;

        getArguments(): Argument[];
        addBasicBlock(basicBlock: BasicBlock);
    }

    export class DataLayout {
        constructor(layout: string);
        getStringRepresentation(): string;
    }

    export class Type {
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

        isDoubleTy(): boolean;
        isVoidTy(): boolean;
    }

    export class FunctionType extends Type {
        static get(result: Type, isVarArg: boolean): FunctionType;
        static get(result: Type, params: Type[], isVarArg: boolean): FunctionType;
        static isValidReturnType(type: Type): boolean;
        static isValidArgumentType(type: Type): boolean;

        returnType: Type;
        isVarArg: boolean;
        numParams: number;
        getParams(): Type[];
        getParamType(index: number): Type;
    }

    export class IRBuilder {
        constructor(context: LLVMContext);

        setInsertionPoint(basicBlock: BasicBlock): void;
        createBr(basicBlock: BasicBlock): Value;
        createCall(callee: Function, args: Value[], name?: string): Value;
        createCondBr(condition: Value, then: BasicBlock, elseBlock: BasicBlock): Value;
        createFAdd(lhs: Value, rhs: Value, name?: string): Value;
        createFCmpOLE(lhs: Value, rhs: Value, name?: string): Value;
        createFSub(lhs: Value ,rhs: Value, name?: string): Value;
        createPhi(type: Type, numReservedValues: number, name?: string): PhiNode;
        createRet(value: Value): Value;
        createRetVoid(): Value;
        getInsertBlock(): BasicBlock;
    }

    export class LLVMContext {
        constructor();
    }

    export class Module {
        sourceFileName: string;
        moduleIdentifier: string;
        targetTriple: string;

        constructor(moduleId: string, context: LLVMContext);
        dump(): void;
        getFunction(name: string): Function;
        setDataLayout(): void;
    }

    // support
    export const TargetRegistry:  {
        lookupTarget(target: string): Target;
    };

    export interface Target {
        name: string;
        shortDescription: string;

        createTargetMachine(triple: string, cpu: string): TargetMachine;
    }

    // target
    export interface TargetMachine {
        createDataLayout(): DataLayout;
    }
}
