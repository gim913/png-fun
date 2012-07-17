#!/bin/bash

FILEZ="encoDeco.o wrapLz4.o wrapLzham.o utiltime.o lz4-read-only/lz4.o lz4-read-only/lz4hc.o lzham-read-only/cmake/liblzhamlib.a"

if [[ $# -lt 1 ]]
then
    echo "need an argument to compile"
    exit 1
fi

echo g++ -O2 -DLODEPNG_WANT_TIMER -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_DECODER=0 -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_ENCODER=2 lodepng.cpp -c -o lodepng.enc.o
echo g++ -O2 -DLODEPNG_WANT_TIMER -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_DECODER=2 -DLODEPNG_OVERRIDE_CUSTOM_ZLIB_ENCODER=0 lodepng.cpp -c -o lodepng.dec.o

g++ -I. -O2 encoDeco.cpp -c
g++ -I. -O2 wrapLz4.cpp -c
g++ -I. -O2 wrapLzham.cpp -c
echo g++ -I. -O2 utiltime.cpp -c

g++ -I. -lpthread -O2 lodepng.enc.o $FILEZ -o bin/png2${1}
g++ -I. -lpthread -O2 lodepng.dec.o $FILEZ -o bin/${1}2png
