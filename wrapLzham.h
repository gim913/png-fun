#pragma once

#include "wrappers.h"

unsigned int lzham_inflate(Gubyte**, size_t*, const gim::pod::MemoryC&, const LodePNGDecompressSettings&);
unsigned int lzham_deflate(Gubyte**, size_t*, const gim::pod::MemoryC&, const LodePNGCompressSettings&);

enum {
    Lzham_None           = 0,
    Lzham_Polar          = 1,
    Lzham_Deterministic  = 4
};

extern int lzham_level;
extern int lzham_flags;
