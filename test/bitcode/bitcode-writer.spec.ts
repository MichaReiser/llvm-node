import * as llvm from "../../";
const fs = require("fs");

const filename = "./bitcode-writer-test.bc";

describe("BitcodeWriter", () => {
    function setupModule() {
        const context = new llvm.LLVMContext();
        const mod = new llvm.Module("test", context);

        return { mod, context };
    }

    beforeEach(() => {
        if (fs.existsSync(filename)) {
            fs.unlinkSync(filename);
        }
    });

    test("writeBitcodeToFile", () => {
        const { mod } = setupModule();
        llvm.writeBitcodeToFile(mod, filename);

        expect(fs.existsSync(filename)).toBe(true);
    });
});