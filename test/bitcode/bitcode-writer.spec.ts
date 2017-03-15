import * as llvm from "../../";
const fs = require("fs");

const filename = "./bitcode-writer-test.bc";

describe("BitcodeWriter", () => {
    let mod: llvm.Module;
    let context: llvm.LLVMContext;

    beforeEach(() => {
        if (fs.existsSync(filename)) {
            fs.unlinkSync(filename);
        }

        context = new llvm.LLVMContext();
        mod = new llvm.Module("test", context);
    });

    test("writeBitcodeToFile", () => {
        llvm.writeBitcodeToFile(mod, filename);

        expect(fs.existsSync(filename)).toBe(true);
    });
});