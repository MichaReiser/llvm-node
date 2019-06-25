#include "attribute-encoding.h"

NAN_MODULE_INIT(InitAttributeEncoding) {
    auto object = Nan::New<v8::Object>();

    Nan::Set(object, Nan::New("address").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_address));;
    Nan::Set(object, Nan::New("boolean").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_boolean));;
    Nan::Set(object, Nan::New("complex_float").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_complex_float));;
    Nan::Set(object, Nan::New("float").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_float));;
    Nan::Set(object, Nan::New("signed").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_signed));;
    Nan::Set(object, Nan::New("signed_char").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_signed_char));;
    Nan::Set(object, Nan::New("unsigned").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_unsigned));;
    Nan::Set(object, Nan::New("unsigned_char").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_unsigned_char));;
    Nan::Set(object, Nan::New("imaginary_float").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_imaginary_float));;
    Nan::Set(object, Nan::New("packed_decimal").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_packed_decimal));;
    Nan::Set(object, Nan::New("numeric_string").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_numeric_string));;
    Nan::Set(object, Nan::New("edited").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_edited));;
    Nan::Set(object, Nan::New("signed_fixed").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_signed_fixed));;
    Nan::Set(object, Nan::New("unsigned_fixed").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_unsigned_fixed));;
    Nan::Set(object, Nan::New("decimal_float").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_decimal_float));;
    Nan::Set(object, Nan::New("UTF").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_UTF));;
    Nan::Set(object, Nan::New("UCS").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_UCS));;
    Nan::Set(object, Nan::New("ASCII").ToLocalChecked(), Nan::New(llvm::dwarf::DW_ATE_ASCII));

    Nan::Set(target, Nan::New("AttributeEncoding").ToLocalChecked(), object);
}
