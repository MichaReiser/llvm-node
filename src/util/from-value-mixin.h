//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_UNWRAP_MIXIN_H
#define LLVM_NODE_UNWRAP_MIXIN_H

#include <napi.h>
#include <uv.h>

template <class T>
class FromValueMixin {
public:
    /**
     * Converts a v8 Value to the wrapper object
     * @param value the v8 value
     * @return the wrapper object
     */
    inline static T* FromValue(Napi::Value value) {
        Napi::Env env = value.Env();
        auto object = value.To<Napi::Object>();
        return object.Unwrap<T>();
    }
};

#endif //LLVM_NODE_UNWRAP_MIXIN_H
