#pragma once

#include "lodepng.h"

unsigned lz4_inflate(unsigned char**, size_t*, const unsigned char*, size_t, const LodePNGDecompressSettings*);
unsigned lz4_deflate(unsigned char**, size_t*, const unsigned char*, size_t, const LodePNGCompressSettings*);

