//
// Created by Micha Reiser on 20.03.17.
//

#ifndef LLVM_NODE_ARRAY_H
#define LLVM_NODE_ARRAY_H

#include<vector>
#include<nan.h>

template <typename T>
std::vector<T> toVector(v8::Local<v8::Array> value) {
    std::vector<T> result (value->Length());

    for (uint32_t i = 0; i < value->Length(); ++i) {
        auto nativeValue = Nan::Get(value, i).ToLocalChecked();
        result[i] = Nan::To<T>(nativeValue).FromJust();
    }

    return result;
}

template <typename T>
std::vector<T> toVector(v8::Local<v8::Value> value) {
    assert(value->IsArray() && "Value needs to be an array");

    return toVector<T>(value.As<v8::Array>());
}

#endif //LLVM_NODE_ARRAY_H