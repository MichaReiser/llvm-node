{
    "variables": {
        "llvm_config": "llvm-config",
    },
    "targets": [
        {
            "target_name": "llvm",
            "sources": [
                "src/llvm-node.cc",
                "src/bitcode/bitcode.cc",
                "src/bitcode/bitcode-writer.cc",
                "src/ir/ir.h",
                "src/ir/ir.cc",
                "src/ir/argument.cc",
                "src/ir/basic-block.cc",
                "src/ir/constants.cc",
                "src/ir/data-layout.cc",
                "src/ir/function.cc",
                "src/ir/function-type.cc",
                "src/ir/ir-builder.cc",
                "src/ir/linkage-types.cc",
                "src/ir/module.cc",
                "src/ir/llvm-context.cc",
                "src/ir/phi-node.cc",
                "src/ir/type.cc",
                "src/ir/value.cc",
                "src/ir/verifier.cc",
                "src/support/support.cc",
                "src/support/target-registry.cc",
                "src/target/target.h",
                "src/target/target-machine.cc",
                "src/util/string.cc",
                "src/util/from-value-mixin.h"
            ],
            "include_dirs" : [
               "<!(node -e \"require('nan')\")",
                "<!@(<(llvm_config) --includedir)"
            ],
            "libraries": [
                "<!@(<(llvm_config) --ldflags)",
                "<!@(<(llvm_config) --libs all)"
            ],
            "cflags": [
                "-O3",
                "-Wall",
                "-Werror",
                "-std=c++14",
            ],
            'conditions': [
                [ 'OS=="mac"', {
                      'xcode_settings': {
                        'OTHER_CPLUSPLUSFLAGS' : ['-stdlib=libc++'],
                        'MACOSX_DEPLOYMENT_TARGET': '10.12'
                      },

                }],
            ]
        }
    ]
}