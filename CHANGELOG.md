#### 1.0.3 (2017-4-20)

##### New Features

* **ConstantInt:** Allow defining unsigned ints (38ddf1ca)
* **Type:** Add getPrimitiveSizeInBits (36332e57)

##### Other Changes

* **Argument:** Adopt addAttr to new llvm signature (a9e140e8)

#### 1.0.2 (2017-4-19)

##### Other Changes

* **VisibilityTypes:** Fix Default visibility type constant (2709ed6b)

#### 1.0.1 (2017-4-14)

##### New Features

* **Function:** Add 'addDereferenceableAttr' method (f2aceb93)

## 1.0.0 (2017-4-12)

##### New Features

* **IRBuilder:** Add 'createSelect' method (52a2efc0)
* **StructType:** Add 'create' method for named structs (4ea9a983)
* **Function:** Add 'visibility' property (14386e2f)
* **CallInst:** Add 'getNumArgOperands' and 'AddDereferenceableAttr' (621457fd)
* **Argument:** Add 'AddAttr' and 'AddDereferenceableAttr' (daddc430)
* **AllocaInst:** Add alignment property (ced1f40a)

#### 0.1.14 (2017-4-9)

##### New Features

* **IRBuilder:** Bitwise Operations (3c5821f9)

#### 0.1.13 (2017-4-8)

##### New Features

* **Module:** Implement getGlobalVariable (42f20706)

#### 0.1.12 (2017-4-5)

##### New Features

* **IRBuilder:**
  * createPtrToInt (854f86cd)
  * createBitCast (767a628c)
* **ConstantArray:** Expose ConstantArray (fe2e9ba2)

#### 0.1.11 (2017-4-3)

##### Other Changes

* **IRBuilder:** CreateCall converts undefined to name (d33aeb7d)

#### 0.1.10 (2017-3-30)

##### New Features

* **IRBuilder:** Expose createNot and createZExt (ac4056f6)

##### Other Changes

* **typings.d.ts:** Fix missing return values (eae7585d)

#### 0.1.9 (2017-3-27)

##### New Features

* **IRBuilder:** FCompOGE, FMul, FNeg, FPToSi, ICmpEQ, ICmpNE, ICmpSGE (eeb5b3dd)

#### 0.1.8 (2017-3-25)

##### New Features

* **GlobalVariable:** Implement Binding for GlobalVariable (40775bf5)

##### Other Changes

* **all:** Expose objects as Classes (af08b311)

#### 0.1.7 (2017-3-24)

##### New Features

* **irbuilder:** createCall (7b402cfa)

#### 0.1.6 (2017-3-23)

##### New Features

* **module:** getOrInsertFunction (c1dece4a)

##### Bug Fixes

* **dependencies:** Make nan a normal dependency (3c2d4742)

### 0.1.0 (2017-3-16)

##### Documentation Changes

* **readme:** Add batch displaying build status (2acd119b)

##### Other Changes

* **dependencies:**
  * Use npm install (50357386)
  * Update Dependencies (00587638)

