cmake_minimum_required(VERSION 3.4)

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
set(CMAKE_CXX_STANDARD 11)

message(STATUS "Using LLVM Version ${LLVM_PACKAGE_VERSION}")

add_library(CallGraphPass MODULE
        # List your source files here.
        main.cpp
#        Graph.cpp
        )

target_compile_features(CallGraphPass PRIVATE cxx_range_for cxx_auto_type)
set_target_properties(CallGraphPass PROPERTIES
        COMPILE_FLAGS "-fno-rtti"
        )

