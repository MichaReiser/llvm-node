cmake_minimum_required(VERSION 3.6)
project(llvm-node)

set(CMAKE_CXX_STANDARD 14)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${CMAKE_JS_INC} PRIVATE "${CMAKE_SOURCE_DIR}/node_modules/node-addon-api" ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
add_definitions(-DNAPI_CPP_EXCEPTIONS=ON)

file(GLOB SOURCE_FILES
        "src/llvm-node.cc"
        "src/bitcode/*.h"
        "src/bitcode/*.cc"
        "src/config/*.h"
        "src/config/*.cc"
        "src/support/*.h"
        "src/support/*.cc"
        "src/target/*.h"
        "src/target/*.cc"
        "src/ir/module.h"
        "src/ir/module.cc"
        "src/ir/llvm-context.h"
        "src/ir/llvm-context.cc"
        "src/ir/ir.h"
        "src/ir/ir.cc"
        "src/ir/constant.h"
        "src/ir/constant.cc"
        "src/ir/type.h"
        "src/ir/type.cc"
        "src/ir/value.h",
        "src/ir/value.cc"
        "src/ir/pointer-type.h"
        "src/ir/pointer-type.cc"
#        "src/*.h"
#        "src/*.cc"
#        "src/**/*.h"
#        "src/**/*.cc"
        )


add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} bitwriter codegen core support tablegen target)

# Link against LLVM libraries
target_link_libraries(${PROJECT_NAME} ${llvm_libs} ${CMAKE_JS_LIB} )