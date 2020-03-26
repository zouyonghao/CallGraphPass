cd ~/Desktop/redis-unstable/src # redis source dir
make > /dev/null 2>&1
clang -Xclang -load -Xclang \
  ~/Desktop/CallGraphPass/cmake-build-debug/DynamicFunctionPass/libDynamicFunctionPass.so\
  redis-cli.c \
  -I../deps/hiredis \
  -I../deps/linenoise \
  -c > /dev/null 2>&1

clang redis-cli.o \
  ../deps/hiredis/*.o \
  ../deps/linenoise/*.o \
  dict.o adlist.o zmalloc.o anet.o ae.o \
  ../deps/jemalloc/lib/libjemalloc.a  \
  -lpthread -lm crc16.o release.o crc64.o siphash.o -ldl \
  ~/Desktop/CallGraphPass/libtest.c > /dev/null 2>&1
./a.out