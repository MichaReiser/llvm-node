//
// Created by Micha Reiser on 11.04.17.
//

#include <llvm/IR/Attributes.h>
#include "attribute.h"

NAN_MODULE_INIT(InitAttribute) {
    auto attribute = Nan::New<v8::Object>();

    auto attrKind = Nan::New<v8::Object>();
    Nan::Set(attrKind, Nan::New("Alignment").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::Alignment)));
    Nan::Set(attrKind, Nan::New("AllocSize").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::AllocSize)));
    Nan::Set(attrKind, Nan::New("AlwaysInline").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::AlwaysInline)));
    Nan::Set(attrKind, Nan::New("ArgMemOnly").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::ArgMemOnly)));
    Nan::Set(attrKind, Nan::New("Builtin").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::Builtin)));
    Nan::Set(attrKind, Nan::New("ByVal").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::ByVal)));
    Nan::Set(attrKind, Nan::New("Cold").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::Cold)));
    Nan::Set(attrKind, Nan::New("Convergent").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::Convergent)));
    Nan::Set(attrKind, Nan::New("Dereferenceable").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::Dereferenceable)));
    Nan::Set(attrKind, Nan::New("DereferenceableOrNull").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::DereferenceableOrNull)));
    Nan::Set(attrKind, Nan::New("EndAttrKinds").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::EndAttrKinds)));
    Nan::Set(attrKind, Nan::New("InaccessibleMemOnly").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::InaccessibleMemOnly)));
    Nan::Set(attrKind, Nan::New("InaccessibleMemOrArgMemOnly").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::InaccessibleMemOrArgMemOnly)));
    Nan::Set(attrKind, Nan::New("InAlloca").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::InAlloca)));
    Nan::Set(attrKind, Nan::New("InlineHint").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::InlineHint)));
    Nan::Set(attrKind, Nan::New("InReg").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::InReg)));
    Nan::Set(attrKind, Nan::New("JumpTable").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::JumpTable)));
    Nan::Set(attrKind, Nan::New("MinSize").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::MinSize)));
    Nan::Set(attrKind, Nan::New("Naked").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::Naked)));
    Nan::Set(attrKind, Nan::New("Nest").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::Nest)));
    Nan::Set(attrKind, Nan::New("NoAlias").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NoAlias)));
    Nan::Set(attrKind, Nan::New("NoInline").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NoInline)));
    Nan::Set(attrKind, Nan::New("NoBuiltin").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NoBuiltin)));
    Nan::Set(attrKind, Nan::New("NoCapture").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NoCapture)));
    Nan::Set(attrKind, Nan::New("NoDuplicate").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NoDuplicate)));
    Nan::Set(attrKind, Nan::New("NoImplicitFloat").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NoImplicitFloat)));
    Nan::Set(attrKind, Nan::New("None").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::None)));
    Nan::Set(attrKind, Nan::New("NonLazyBind").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NonLazyBind)));
    Nan::Set(attrKind, Nan::New("NonNull").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NonNull)));
    Nan::Set(attrKind, Nan::New("NoRecurse").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NoRecurse)));
    Nan::Set(attrKind, Nan::New("NoRedZone").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NoRedZone)));
    Nan::Set(attrKind, Nan::New("NoReturn").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NoReturn)));
    Nan::Set(attrKind, Nan::New("NoUnwind").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::NoUnwind)));
    Nan::Set(attrKind, Nan::New("OptimizeForSize").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::OptimizeForSize)));
    Nan::Set(attrKind, Nan::New("OptimizeNone").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::OptimizeNone)));
    Nan::Set(attrKind, Nan::New("ReadNone").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::ReadNone)));
    Nan::Set(attrKind, Nan::New("ReadOnly").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::ReadOnly)));
    Nan::Set(attrKind, Nan::New("Returned").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::Returned)));
    Nan::Set(attrKind, Nan::New("ReturnsTwice").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::ReturnsTwice)));
    Nan::Set(attrKind, Nan::New("SafeStack").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::SafeStack)));
    Nan::Set(attrKind, Nan::New("SanitizeAddress").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::SanitizeAddress)));
    Nan::Set(attrKind, Nan::New("SanitizeMemory").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::SanitizeMemory)));
    Nan::Set(attrKind, Nan::New("SanitizeThread").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::SanitizeThread)));
    Nan::Set(attrKind, Nan::New("SExt").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::SExt)));
    Nan::Set(attrKind, Nan::New("StackAlignment").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::StackAlignment)));
    Nan::Set(attrKind, Nan::New("StackProtect").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::StackProtect)));
    Nan::Set(attrKind, Nan::New("StackProtectReq").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::StackProtectReq)));
    Nan::Set(attrKind, Nan::New("StackProtectStrong").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::StackProtectStrong)));
    Nan::Set(attrKind, Nan::New("SafeStack").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::SafeStack)));
    Nan::Set(attrKind, Nan::New("StructRet").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::StructRet)));
    Nan::Set(attrKind, Nan::New("SwiftError").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::SwiftError)));
    Nan::Set(attrKind, Nan::New("SwiftSelf").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::SwiftSelf)));
    Nan::Set(attrKind, Nan::New("UWTable").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::UWTable)));
    Nan::Set(attrKind, Nan::New("WriteOnly").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::WriteOnly)));
    Nan::Set(attrKind, Nan::New("ZExt").ToLocalChecked(), Nan::New(static_cast<uint32_t>(llvm::Attribute::AttrKind::ZExt)));

    Nan::Set(attribute, Nan::New("AttrKind").ToLocalChecked(), attrKind);

    Nan::Set(target, Nan::New("Attribute").ToLocalChecked(), attribute);
}