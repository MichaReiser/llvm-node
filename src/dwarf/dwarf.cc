#include <llvm/IR/Attributes.h>
#include "source-language.h"
#include "attribute-encoding.h"

NAN_MODULE_INIT(InitDwarf) {
    auto dwarf = Nan::New<v8::Object>();

    InitSourceLanguage(dwarf);
    InitAttributeEncoding(dwarf);

    Nan::Set(target, Nan::New("dwarf").ToLocalChecked(), dwarf);
}
