import * as llvm from "../../";
import { createModule, createBuilder } from "../test-utils";

describe("ir/module", () => {
  describe("constructor", () => {
    const untypedCtor = llvm.Module as any;
    test("can create a new module", () => {
      const mod = new llvm.Module("test", new llvm.LLVMContext());
      expect(mod).toBeDefined();
      expect(mod.moduleIdentifier).toBe("test");
    });

    test("throws if called with not enough arguments", () => {
      expect(() => new untypedCtor()).toThrowError(
        "The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)"
      );
      expect(() => new untypedCtor("test")).toThrowError(
        "The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)"
      );
    });

    test("throws with arguments not matching the expected type", () => {
      expect(() => new untypedCtor(1, new llvm.LLVMContext())).toThrowError(
        "The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)"
      );
      expect(() => new untypedCtor("test", {})).toThrowError(
        "The Module functionTemplate needs to be called with: (moduleId: string, context: LLVMContext)"
      );
    });
  });

  describe("empty", () => {
    it("returns true when the module is empty", () => {
      const { module } = createModule();

      expect(module.empty).toBe(true);
    });

    it("returns false when the module is no longer empty", () => {
      const { module, context } = createModule();

      const fnType = llvm.FunctionType.get(llvm.Type.getVoidTy(context), false);
      const fn = llvm.Function.create(fnType, llvm.LinkageTypes.ExternalWeakLinkage, "test", module);

      expect(module.empty).toBe(false);
    });
  });

  describe("name", () => {
    it("returns the name of the module", () => {
      const { module } = createModule({ moduleId: "abcd" });

      expect(module.name).toBe("abcd");
    });
  });

  describe("moduleIdentifier", () => {
    test("returns the module identifier", () => {
      const { module } = createModule({ moduleId: "abcd" });

      expect(module.moduleIdentifier).toBe("abcd");
    });

    test("sets the moduleIdentifier", () => {
      const { module } = createModule();
      module.moduleIdentifier = "fib";
      expect(module.moduleIdentifier).toBe("fib");
    });

    test("it throws if it's not a string", () => {
      const { module } = createModule();

      expect(() => (module.moduleIdentifier = 2 as any)).toThrowError("moduleIdentifier needs to be a string");
    });
  });

  describe("sourceFileName", () => {
    test("sets the source file name", () => {
      const { module } = createModule();
      module.sourceFileName = "fib.js";
      expect(module.sourceFileName).toBe("fib.js");
    });

    test("it throws if it's not a string", () => {
      const { module } = createModule();

      expect(() => (module.sourceFileName = 2 as any)).toThrowError("sourceFileName needs to be a string");
    });
  });

  describe("targetTriple", () => {
    test("sets the target triple", () => {
      const { module } = createModule();

      module.targetTriple = "wasm32-unknown-unknown";
      expect(module.targetTriple).toBe("wasm32-unknown-unknown");
    });

    test("it throws if it's not a string", () => {
      const { module } = createModule();

      expect(() => (module.targetTriple = 1 as any)).toThrowError("targetTriple needs to be a string");
    });

    it("it throws if it is an illegal target triple", () => {
      const { module } = createModule();

      module.targetTriple = "test";
    });
  });

  if (llvm.Module.prototype.dump) {
    describe("dump", () => {
      test("dumps the module", () => {
        const mod = new llvm.Module("test", new llvm.LLVMContext());
        mod.dump!();
      });
    });
  }

  describe("getFunction", () => {
    test("returns the function with the given name", () => {
      const { module, context } = createModule();

      const fun = llvm.Function.create(
        llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [], false),
        llvm.LinkageTypes.ExternalLinkage,
        "sin",
        module
      );
      expect(module.getFunction("sin")).toEqual(fun);
    });

    test("returns undefined if no such function exists", () => {
      const { module } = createModule();

      expect(module.getFunction("fib")).toBeUndefined();
    });
  });

  describe("getOrInsertFunction", () => {
    test("returns a new function if not yet existing in the module", () => {
      const { module, context } = createModule();
      const functionType = llvm.FunctionType.get(
        llvm.Type.getDoubleTy(context),
        [llvm.Type.getDoubleTy(context)],
        false
      );

      const functionCallee = module.getOrInsertFunction("fib", functionType);

      expect(functionCallee).toBeDefined();
      expect(module.getFunction("fib")).toEqual(functionCallee.callee);
      expect(functionCallee.functionType).toEqual(functionType);
    });

    test("returns the existing function if a function with the given name already exist", () => {
      const { module, context } = createModule();

      const functionType = llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [], false);
      const fun = llvm.Function.create(functionType, llvm.LinkageTypes.ExternalLinkage, "sin", module);

      const functionCallee = module.getOrInsertFunction(
        "sin",
        llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [], false)
      );

      expect(functionCallee.callee).toEqual(fun);
      expect(functionCallee.functionType).toEqual(functionType);
    });
  });

  describe("print", () => {
    test("returns the llvm assembly code for the module", () => {
      const { module, context } = createModule();

      const fun = llvm.Function.create(
        llvm.FunctionType.get(llvm.Type.getDoubleTy(context), [], false),
        llvm.LinkageTypes.ExternalLinkage,
        "sin",
        module
      );

      const entry = llvm.BasicBlock.create(context, "entry", fun);

      expect(module.print()).toMatchSnapshot();
    });
  });

  describe("getGlobalVariable", () => {
    it("returns undefined if the global variable is not defined", () => {
      const { module } = createModule();

      expect(module.getGlobalVariable("notExisting")).toBeUndefined();
    });

    it("returns the defined global variable", () => {
      const { builder, module, context } = createBuilder();
      const global = new llvm.GlobalVariable(
        module,
        llvm.Type.getInt32Ty(context),
        true,
        llvm.LinkageTypes.ExternalLinkage,
        llvm.ConstantInt.get(context, 2),
        "globalVar"
      );

      expect(module.getGlobalVariable("globalVar"));
    });
  });

  describe("getTypeByName", () => {
    test("returns null if a type with the given name does not exist", () => {
      const { module, context } = createModule();

      const type = module.getTypeByName("DoesNotExist");

      expect(type).toBeNull();
    });

    test("returns the type with the given name if it exists", () => {
      const { module, context } = createModule();
      const structType = llvm.StructType.create(context, "IntTuple");
      structType.setBody([llvm.Type.getInt32Ty(context), llvm.Type.getInt32Ty(context)]);

      const retrieved = module.getTypeByName("IntTuple");

      expect(retrieved).toBeInstanceOf(llvm.StructType);
      expect(retrieved).toEqual(structType);
    });
  });
});
