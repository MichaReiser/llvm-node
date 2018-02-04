import * as llvm from "../../";
import {createModule} from "../test-utils";
const fs = require("fs");

const filename = "./bitcode-writer-test.bc";

describe("BitcodeWriter", () => {
    beforeEach(() => {
        if (fs.existsSync(filename)) {
            fs.unlinkSync(filename);
        }
    });

    test("writeBitcodeToFile", () => {
        const { module } = createModule();
        llvm.writeBitcodeToFile(module, filename);

        expect(fs.existsSync(filename)).toBe(true);
    });
});