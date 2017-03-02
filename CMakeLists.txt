cmake_minimum_required(VERSION 3.6)
project(llvm_node)

set(CMAKE_CXX_STANDARD 14)

file(GLOB llvm_node_SRC
        "src/*.h"
        "src/*.cc"
        "src/**/*.h"
        "src/**/*.cc"
        )

set(SOURCE_FILES ${llvm_node_SRC})

include(./node_modules/node-cmake/NodeJS.cmake)
nodejs_init()

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS} "./node_modules/nan")
add_definitions(${LLVM_DEFINITIONS})
add_nodejs_module(${PROJECT_NAME} ${SOURCE_FILES})

llvm_map_components_to_libnames(llvm_libs all)

# Link against LLVM libraries
target_link_libraries(${PROJECT_NAME} ${llvm_libs})