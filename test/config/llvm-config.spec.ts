import * as llvm from "../../";

describe("llvm-config", () => {
    test("LLVM_VERSION_MAJOR has a value", () => {
        expect(llvm.config.LLVM_VERSION_MAJOR).toEqual(expect.any(Number));
    });

    test("LLVM_VERSION_MINOR has a value", () => {
        expect(llvm.config.LLVM_VERSION_MINOR).toEqual(expect.any(Number));
    });

    test("LLVM_VERSION_PATCH has a value", () => {
        expect(llvm.config.LLVM_VERSION_PATCH).toEqual(expect.any(Number));
    });

    test("LLVM_VERSION_STRING has a value", () => {
        expect(llvm.config.LLVM_VERSION_STRING).toEqual(expect.any(String));
    });

    test("LLVM_DEFAULT_TARGET_TRIPLE has a value", () => {
        expect(llvm.config.LLVM_DEFAULT_TARGET_TRIPLE).toEqual(expect.any(String));
    });
});