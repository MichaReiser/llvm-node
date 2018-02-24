//
// Created by Micha Reiser on 11.04.17.
//

#include <llvm/IR/Attributes.h>
#include "attribute.h"

Napi::Object InitAttribute(Napi::Env env, Napi::Object exports) {
    auto attribute = Napi::Object::New(env);

    auto attrKind = Napi::Object::New(env);
    (attrKind).Set(Napi::String::New(env, "Alignment"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::Alignment)));
    (attrKind).Set(Napi::String::New(env, "AllocSize"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::AllocSize)));
    (attrKind).Set(Napi::String::New(env, "AlwaysInline"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::AlwaysInline)));
    (attrKind).Set(Napi::String::New(env, "ArgMemOnly"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::ArgMemOnly)));
    (attrKind).Set(Napi::String::New(env, "Builtin"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::Builtin)));
    (attrKind).Set(Napi::String::New(env, "ByVal"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::ByVal)));
    (attrKind).Set(Napi::String::New(env, "Cold"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::Cold)));
    (attrKind).Set(Napi::String::New(env, "Convergent"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::Convergent)));
    (attrKind).Set(Napi::String::New(env, "Dereferenceable"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::Dereferenceable)));
    (attrKind).Set(Napi::String::New(env, "DereferenceableOrNull"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::DereferenceableOrNull)));
    (attrKind).Set(Napi::String::New(env, "EndAttrKinds"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::EndAttrKinds)));
    (attrKind).Set(Napi::String::New(env, "InaccessibleMemOnly"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::InaccessibleMemOnly)));
    (attrKind).Set(Napi::String::New(env, "InaccessibleMemOrArgMemOnly"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::InaccessibleMemOrArgMemOnly)));
    (attrKind).Set(Napi::String::New(env, "InAlloca"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::InAlloca)));
    (attrKind).Set(Napi::String::New(env, "InlineHint"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::InlineHint)));
    (attrKind).Set(Napi::String::New(env, "InReg"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::InReg)));
    (attrKind).Set(Napi::String::New(env, "JumpTable"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::JumpTable)));
    (attrKind).Set(Napi::String::New(env, "MinSize"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::MinSize)));
    (attrKind).Set(Napi::String::New(env, "Naked"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::Naked)));
    (attrKind).Set(Napi::String::New(env, "Nest"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::Nest)));
    (attrKind).Set(Napi::String::New(env, "NoAlias"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NoAlias)));
    (attrKind).Set(Napi::String::New(env, "NoInline"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NoInline)));
    (attrKind).Set(Napi::String::New(env, "NoBuiltin"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NoBuiltin)));
    (attrKind).Set(Napi::String::New(env, "NoCapture"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NoCapture)));
    (attrKind).Set(Napi::String::New(env, "NoDuplicate"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NoDuplicate)));
    (attrKind).Set(Napi::String::New(env, "NoImplicitFloat"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NoImplicitFloat)));
    (attrKind).Set(Napi::String::New(env, "None"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::None)));
    (attrKind).Set(Napi::String::New(env, "NonLazyBind"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NonLazyBind)));
    (attrKind).Set(Napi::String::New(env, "NonNull"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NonNull)));
    (attrKind).Set(Napi::String::New(env, "NoRecurse"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NoRecurse)));
    (attrKind).Set(Napi::String::New(env, "NoRedZone"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NoRedZone)));
    (attrKind).Set(Napi::String::New(env, "NoReturn"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NoReturn)));
    (attrKind).Set(Napi::String::New(env, "NoUnwind"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::NoUnwind)));
    (attrKind).Set(Napi::String::New(env, "OptimizeForSize"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::OptimizeForSize)));
    (attrKind).Set(Napi::String::New(env, "OptimizeNone"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::OptimizeNone)));
    (attrKind).Set(Napi::String::New(env, "ReadNone"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::ReadNone)));
    (attrKind).Set(Napi::String::New(env, "ReadOnly"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::ReadOnly)));
    (attrKind).Set(Napi::String::New(env, "Returned"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::Returned)));
    (attrKind).Set(Napi::String::New(env, "ReturnsTwice"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::ReturnsTwice)));
    (attrKind).Set(Napi::String::New(env, "SafeStack"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::SafeStack)));
    (attrKind).Set(Napi::String::New(env, "SanitizeAddress"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::SanitizeAddress)));
    (attrKind).Set(Napi::String::New(env, "SanitizeMemory"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::SanitizeMemory)));
    (attrKind).Set(Napi::String::New(env, "SanitizeThread"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::SanitizeThread)));
    (attrKind).Set(Napi::String::New(env, "SExt"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::SExt)));
    (attrKind).Set(Napi::String::New(env, "StackAlignment"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::StackAlignment)));
    (attrKind).Set(Napi::String::New(env, "StackProtect"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::StackProtect)));
    (attrKind).Set(Napi::String::New(env, "StackProtectReq"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::StackProtectReq)));
    (attrKind).Set(Napi::String::New(env, "StackProtectStrong"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::StackProtectStrong)));
    (attrKind).Set(Napi::String::New(env, "SafeStack"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::SafeStack)));
    (attrKind).Set(Napi::String::New(env, "StructRet"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::StructRet)));
    (attrKind).Set(Napi::String::New(env, "SwiftError"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::SwiftError)));
    (attrKind).Set(Napi::String::New(env, "SwiftSelf"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::SwiftSelf)));
    (attrKind).Set(Napi::String::New(env, "UWTable"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::UWTable)));
    (attrKind).Set(Napi::String::New(env, "WriteOnly"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::WriteOnly)));
    (attrKind).Set(Napi::String::New(env, "ZExt"), Napi::New(env, static_cast<uint32_t>(llvm::Attribute::AttrKind::ZExt)));

    (attribute).Set(Napi::String::New(env, "AttrKind"), attrKind);

    (target).Set(Napi::String::New(env, "Attribute"), attribute);
}