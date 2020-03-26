cd ~/Desktop/redis-unstable/src # redis source dir
make
clang -Xclang -load -Xclang \
  ~/Desktop/CallGraphPass/cmake-build-debug/StaticFunctionPass/libStaticFunctionPass.so \
  redis-cli.c \
  -I../deps/hiredis \
  -I../deps/linenoise \
  ../deps/hiredis/*.o \
  ../deps/linenoise/*.o \
  dict.o adlist.o zmalloc.o anet.o ae.o \
  ../deps/jemalloc/lib/libjemalloc.a  \
  -lpthread -lm crc16.o release.o crc64.o siphash.o -ldl