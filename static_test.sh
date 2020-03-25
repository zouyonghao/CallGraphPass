mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
make
cd ..
clang -Xclang -load -Xclang ./cmake-build-debug/StaticFunctionPass/libStaticFunctionPass.so test.c