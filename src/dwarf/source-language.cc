#include "source-language.h"

NAN_MODULE_INIT(InitSourceLanguage) {
    auto object = Nan::New<v8::Object>();

    Nan::Set(object, Nan::New("C89").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_C89));;
    Nan::Set(object, Nan::New("C").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_C));;
    Nan::Set(object, Nan::New("Ada83").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Ada83));;
    Nan::Set(object, Nan::New("C_plus_plus").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_C_plus_plus));;
    Nan::Set(object, Nan::New("Cobol74").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Cobol74));;
    Nan::Set(object, Nan::New("Cobol85").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Cobol85));;
    Nan::Set(object, Nan::New("Fortran77").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Fortran77));;
    Nan::Set(object, Nan::New("Fortran90").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Fortran90));;
    Nan::Set(object, Nan::New("Pascal83").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Pascal83));;
    Nan::Set(object, Nan::New("Modula2").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Modula2));;
    Nan::Set(object, Nan::New("Java").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Java));;
    Nan::Set(object, Nan::New("C99").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_C99));;
    Nan::Set(object, Nan::New("Ada95").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Ada95));;
    Nan::Set(object, Nan::New("Fortran95").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Fortran95));;
    Nan::Set(object, Nan::New("PLI").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_PLI));;
    Nan::Set(object, Nan::New("ObjC").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_ObjC));;
    Nan::Set(object, Nan::New("ObjC_plus_plus").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_ObjC_plus_plus));;
    Nan::Set(object, Nan::New("UPC").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_UPC));;
    Nan::Set(object, Nan::New("D").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_D));;
    Nan::Set(object, Nan::New("Python").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Python));;
    Nan::Set(object, Nan::New("OpenCL").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_OpenCL));;
    Nan::Set(object, Nan::New("Go").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Go));;
    Nan::Set(object, Nan::New("Modula3").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Modula3));;
    Nan::Set(object, Nan::New("Haskell").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Haskell));;
    Nan::Set(object, Nan::New("C_plus_plus_03").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_C_plus_plus_03));;
    Nan::Set(object, Nan::New("C_plus_plus_11").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_C_plus_plus_11));;
    Nan::Set(object, Nan::New("OCaml").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_OCaml));;
    Nan::Set(object, Nan::New("Rust").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Rust));;
    Nan::Set(object, Nan::New("C11").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_C11));;
    Nan::Set(object, Nan::New("Swift").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Swift));;
    Nan::Set(object, Nan::New("Julia").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Julia));;
    Nan::Set(object, Nan::New("Dylan").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Dylan));;
    Nan::Set(object, Nan::New("C_plus_plus_14").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_C_plus_plus_14));;
    Nan::Set(object, Nan::New("Fortran03").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Fortran03));;
    Nan::Set(object, Nan::New("Fortran08").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_Fortran08));;
    Nan::Set(object, Nan::New("RenderScript").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_RenderScript));;
    Nan::Set(object, Nan::New("BLISS").ToLocalChecked(), Nan::New(llvm::dwarf::DW_LANG_BLISS));

    Nan::Set(target, Nan::New("SourceLanguage").ToLocalChecked(), object);
}
