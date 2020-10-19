//
// Created by lx.codev on 10.07.2020.
//

#include "jit.h"

#include "kaleidoscope-jit.h"
#include "jit-symbol.h"

NAN_MODULE_INIT(InitJit) {
  KaleidoscopeJitWrapper::Init(target);
  JitSymbolWrapper::Init(target);
}