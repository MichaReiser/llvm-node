# llvm-node

[![Build Status](https://travis-ci.org/MichaReiser/llvm-node.svg?branch=master)](https://travis-ci.org/MichaReiser/llvm-node)
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

To build llvm-node against a custom --- not the systems default --- LLVM installation you can set the `GYP_DEFINES` variable before running `npm install` as follow:

```
GYP_DEFINES="LLVM_CONFIG=/path/to/llvm-config"
```

The `LLVM_CONFIG` value must point to the `llvm-config` executable. 

