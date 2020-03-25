mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
make
cd ..
clang -Xclang -load -Xclang ./cmake-build-debug/DynamicFunctionPass/libDynamicFunctionPass.so test.c -c
clang libtest.c test.o
./a.out