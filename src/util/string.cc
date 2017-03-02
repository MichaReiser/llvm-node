//
// Created by Micha Reiser on 28.02.17.
//

#include "string.h"

std::string ToString(v8::Local<v8::Value> value) {
    Nan::Utf8String utf8Value { value };
    return std::string {*utf8Value};
}