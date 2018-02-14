import * as llvm from "../../";
import { createBuilderWithBlock } from "../test-utils";

describe("PhiNode", () => {
  describe("addIncoming", () => {
    it("adds an incoming basic block", () => {
      const { builder, context } = createBuilderWithBlock();
      const phi = builder.createPhi(llvm.Type.getDoubleTy(context), 2, "join");
      phi.addIncoming(llvm.ConstantFP.get(context, 2), llvm.BasicBlock.create(context));
    });

    it("throws if the first argument is not a value", () => {
      const { builder, context } = createBuilderWithBlock();

      const phi = builder.createPhi(llvm.Type.getDoubleTy(context), 2, "join");
      expect(() => (phi.addIncoming as any)({})).toThrowError(
        "addIncome needs to be called with: value: Value, basicBlock: BasicBlock"
      );
    });

    it("throws if the second argument is not a basic block", () => {
      const { builder, context } = createBuilderWithBlock();

      const phi = builder.createPhi(llvm.Type.getDoubleTy(context), 2, "join");
      const value = llvm.ConstantFP.get(context, 2);
      expect(() => (phi.addIncoming as any)(value, {})).toThrowError(
        "addIncome needs to be called with: value: Value, basicBlock: BasicBlock"
      );
    });
  });
});
