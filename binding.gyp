{
    "variables": {
        "LLVM_CONFIG%": "llvm-config",
    },
    "targets": [
        {
            "target_name": "llvm",
            "default_configuration": "Release",
            "sources": [
                "src/llvm-node.cc",
                "src/bitcode/bitcode.cc",
                "src/bitcode/bitcode-writer.cc",
                "src/ir/ir.h",
                "src/ir/ir.cc",
                "src/ir/attribute.cc",
                "src/ir/alloca-inst.cc",
                "src/ir/array-type.cc",
                "src/ir/argument.cc",
                "src/ir/basic-block.cc",
                "src/ir/call-inst.cc",
                "src/ir/calling-conv.cc",
                "src/ir/constant.cc",
                "src/ir/constant-array.cc",
                "src/ir/constant-data-array.cc",
                "src/ir/constant-fp.cc",
                "src/ir/constant-int.cc",
                "src/ir/constant-pointer-null.cc",
                "src/ir/constant-struct.cc",
                "src/ir/data-layout.cc",
                "src/ir/function.cc",
                "src/ir/function-type.cc",
                "src/ir/global-variable.cc",
                "src/ir/ir-builder.cc",
                "src/ir/linkage-types.cc",
                "src/ir/module.cc",
                "src/ir/llvm-context.cc",
                "src/ir/phi-node.cc",
                "src/ir/pointer-type.cc",
                "src/ir/struct-type.cc",
                "src/ir/type.cc",
                "src/ir/undef-value.cc",
                "src/ir/unnamed-addr.cc",
                "src/ir/value.cc",
                "src/ir/verifier.cc",
                "src/ir/visibility-types.cc",
                "src/support/support.cc",
                "src/support/target-registry.cc",
                "src/target/target.h",
                "src/target/target-machine.cc",
                "src/util/string.cc",
                "src/util/from-value-mixin.h"
            ],
            "include_dirs" : [
                "<!(node -e \"require('nan')\")",
                "<!@(<(LLVM_CONFIG) --includedir)"
            ],
            "libraries": [
                "<!@(<(LLVM_CONFIG) --ldflags)",
                "<!@(<(LLVM_CONFIG) --libs all)"
            ],
            "cflags": [
                "-Wall",
                "-Werror",
                "-std=c++11",
                "-c <!@(<(LLVM_CONFIG) --cppflags)"
            ],
            "conditions": [
                [ 'OS=="mac"', {
                      "xcode_settings": {
                        "OTHER_CPLUSPLUSFLAGS" : ["-stdlib=libc++"],
                        "MACOSX_DEPLOYMENT_TARGET": "10.12"
                      }

                }],
            ],
            "configurations": {
                "Debug": {
                    "defines": [ "LLVM_NODE_DEBUG", "_DEBUG" ]
                },
                "Release": {
                    "cflags": [
                        "-O3"
                    ]
                }
            }
        }
    ]
}