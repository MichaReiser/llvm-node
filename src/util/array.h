//
// Created by Micha Reiser on 20.03.17.
//

#ifndef LLVM_NODE_ARRAY_H
#define LLVM_NODE_ARRAY_H

#include<vector>
#include<napi.h>

template <typename T, typename TArray=v8::Array>
std::vector<T> toVector(v8::Local<TArray> value) {
    std::vector<T> result (value->Length());

    for (uint32_t i = 0; i < value->Length(); ++i) {
        auto nativeValue = (value).Get(i);
        result[i] = Napi::To<T>(nativeValue);
    }

    return result;
}

template <typename T, typename TArray=v8::Array>
std::vector<T> toVector(Napi::Value value) {
    Napi::Env env = value.Env();
    return toVector<T>(value.As<TArray>());
}

#endif //LLVM_NODE_ARRAY_H