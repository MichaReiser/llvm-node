//
// Created by Micha Reiser on 01.03.17.
//

#ifndef LLVM_NODE_UNWRAP_MIXIN_H
#define LLVM_NODE_UNWRAP_MIXIN_H

#include <nan.h>

template <class T>
class FromValueMixin {
public:
    /**
     * Converts a v8 Value to the wrapper object
     * @param value the v8 value
     * @return the wrapper object
     */
    inline static T* FromValue(v8::Local<v8::Value> value) {
        auto object = Nan::To<v8::Object>(value).ToLocalChecked();
        return Nan::ObjectWrap::Unwrap<T>(object);
    }
};

#endif //LLVM_NODE_UNWRAP_MIXIN_H
