# FunctionPass example

## Build
  * ``cd mkdir build``
  * ``export LLVM_DIR=/path/to/llvm-build/lib/cmake/llvm``
  * ``cmake ..``
  * ``make``

## Run test
  * ``clang -Xclang -load -Xclang MyFunctionPass/libMyFunctionPass.so test.c``
