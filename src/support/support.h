#ifndef SUPPORT_H
#define SUPPORT_H
#include <napi.h>
#include <uv.h>
#include "target-registry.h"

Napi::Value InitializeAllTargetInfos(const Napi::CallbackInfo& info);
Napi::Value InitializeAllTargets(const Napi::CallbackInfo& info);
Napi::Value InitializeAllTargetMCs(const Napi::CallbackInfo& info);
Napi::Value InitializeAllAsmParsers(const Napi::CallbackInfo& info);
Napi::Value InitializeAllAsmPrinters(const Napi::CallbackInfo& info);

Napi::Object InitSupport(Napi::Env env, Napi::Object exports);

#endif