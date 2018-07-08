//
// Created by Micha Reiser on 08.07.18.
//

#ifndef LLVM_NODE_INHERIT_H
#define LLVM_NODE_INHERIT_H

#include <napi.h>
#include "../../node_modules/node-addon-api/napi.h"

void Inherit(Napi::Env env, Napi::Function childCtor, Napi::Function superCtor) {

    Napi::Object global = env.Global();
    Napi::Object Object = global.Get("Object").As<Napi::Object>();
    Napi::Function setPrototypeOf = Object.Get("setPrototypeOf").As<Napi::Function>();

    Napi::Value childProto = childCtor.Get("prototype");
    Napi::Value superProto = superCtor.Get("prototype");

    setPrototypeOf.Call({ childProto, superProto });
    setPrototypeOf.Call({ childCtor, superCtor });
}

#endif //LLVM_NODE_INHERIT_H
