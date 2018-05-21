# llvm-node

[![Build Status](https://travis-ci.com/MichaReiser/llvm-node.svg?branch=master)](https://travis-ci.com/MichaReiser/llvm-node)
[![npm version](https://badge.fury.io/js/llvm-node.svg)](https://badge.fury.io/js/llvm-node)

Node Bindings for LLVM. The API is mostly identical to the one of LLVM (when possible and not to cumbersom). All supported methods with their signatures are listed in the [TypeScript definition file](./llvm-node.d.ts). 

## Getting Started

Install the packages using npm or yarn.

```bash
npm install --save llvm-node
```

Require llvm-node and you're good to go

```js
import * as llvm from "llvm-node";
// or
const llvm = require("llvm-node");

const context = new llvm.LLVMContext();
const module = new llvm.Module("test", context);

const intType = llvm.Type.getInt32Ty(context);
const initializer = llvm.ConstantInt.get(context, 0);
const globalVariable = new llvm.GlobalVariable(module, intType, true, llvm.LinkageTypes.InternalLinkage, initializer);

const ll = module.print(); // prints IR
llvm.writeBitcodeToFile(module, biteCodeFileName); // Writes file to disk
```

## Custom LLVM Installation
You can use the npm configuration options to set the path to the LLVM cmake directory. This is needed if you don't want to use the system default LLVM installation --- e.g. when you want to compile againt the latest dev version.

To change the LLVM version globally, use:

```sh
npm config set --global cmake_LLVM_DIR $(path-to-llvm/bin/llvm-config --cmakedir)
```

or for the current user only

```sh
npm config set cmake_LLVM_DIR $(path-to-llvm/bin/llvm-config --cmakedir)
```

If you want to change the value just for a single project use an [.npmrc](https://docs.npmjs.com/files/npmrc) file in your project.

## Debug Build
If you want to use `Value.dump` or `Module.dump` you need to use the debug build of the library (and, as well, of LLVM).
In this case, run `node-gyp build --debug` or if using `npm install` run `npm_config_DEBUG=true npm install llvm-node`.

There variables can also be set using npm set:

`npm config set [--global] DEBUG True`
