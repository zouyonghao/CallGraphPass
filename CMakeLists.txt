cmake_minimum_required(VERSION 3.4)

find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(
        ${LLVM_INCLUDE_DIRS}
        include
)
link_directories(${LLVM_LIBRARY_DIRS})
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(StaticFunctionPass)
add_subdirectory(DynamicFunctionPass)

ADD_LIBRARY(LIBTEST libtest.c)