//
// Created by Micha Reiser on 28.02.17.
//

#include "string.h"

std::string ToString(Napi::Value value) {
    Napi::Env env = value.Env();
    std::string utf8Value { value };
    return std::string {*utf8Value};
}