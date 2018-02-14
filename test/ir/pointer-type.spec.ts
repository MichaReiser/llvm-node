import * as llvm from "../..";
import { createModule } from "../test-utils";

describe("ir/pointer-type", () => {
  it("inherits type", () => {
    const { context } = createModule();

    const type = llvm.PointerType.get(llvm.Type.getInt32Ty(context), 0);

    expect(type.typeID).toBe(llvm.Type.TypeID.PointerTyID);
  });

  describe("get", () => {
    it("returns a pointer type", () => {
      const { context } = createModule();

      const type = llvm.PointerType.get(llvm.Type.getInt32Ty(context), 0);

      expect(type).toBeInstanceOf(llvm.PointerType);
      expect(type).toEqual(llvm.Type.getInt32PtrTy(context));
    });
  });

  describe("elementType", () => {
    it("returns the type of the element", () => {
      const { context } = createModule();

      const ptrType = llvm.PointerType.get(llvm.Type.getInt32Ty(context), 0);

      expect(ptrType.elementType).toEqual(llvm.Type.getInt32Ty(context));
    });
  });
});
