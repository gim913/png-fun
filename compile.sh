#!/bin/bash

FILEZ="encoDeco.cpp wrapLz4.cpp wrapLzham.cpp utiltime.cpp lz4-read-only/lz4.o lz4-read-only/lz4hc.o lzham-read-only/cmake/liblzhamlib.a"

g++ -O2 -DLODEPNG_WANT_TIMER -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_DECODER=0 -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_ENCODER=2 lodepng.cpp -c -o lodepng.enc.o
g++ -O2 -DLODEPNG_WANT_TIMER -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_DECODER=2 -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_ENCODER=0 lodepng.cpp -c -o lodepng.dec.o
g++ -I. -lpthread -O2 lodepng.enc.o $FILEZ -o bin/png2lz4
g++ -I. -lpthread -O2 lodepng.dec.o $FILEZ -o bin/lz42png
