#!/bin/bash

FILEZ="encoDeco.o wrapLz4.o wrapLzham.o wrapBsc.o wrapDaSh.o wrapSnappy.o utiltime.o"
LIBZ="lz4-read-only/lz4.o lz4-read-only/lz4hc.o lzham-read-only/cmake/liblzhamlib.a libbsc-0b12f29/libbsc.a data-shrinker-read-only/Shrinker.o snappy-read-only/.libs/libsnappy.a"

g++ -O2 -DLODEPNG_WANT_TIMER -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_DECODER=0 -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_ENCODER=2 lodepng.cpp -c -o lodepng.enc.o
g++ -O2 -DLODEPNG_WANT_TIMER -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_DECODER=2 -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_ENCODER=0 lodepng.cpp -c -o lodepng.dec.o

g++ -I. -O2 encoDeco.cpp -c
g++ -I. -O2 wrapLz4.cpp -c
g++ -I. -O2 wrapLzham.cpp -c
g++ -I. -O2 wrapBsc.cpp -c
g++ -I. -O2 wrapDaSh.cpp -c
g++ -I. -O2 wrapSnappy.cpp -c
g++ -I. -O2 utiltime.cpp -c

g++ -I. -lpthread -O2 lodepng.enc.o $FILEZ $LIBZ -o bin/png2cus
g++ -I. -lpthread -O2 lodepng.dec.o $FILEZ $LIBZ -o bin/cus2png
