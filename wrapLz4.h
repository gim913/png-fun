#pragma once

#include "wrappers.h"
#include "lodepng.h"

unsigned lz4_inflate(Gubyte**, size_t*, const gim::pod::MemoryC&, const LodePNGDecompressSettings&);
unsigned lz4_deflate(Gubyte**, size_t*, const gim::pod::MemoryC&, const LodePNGCompressSettings&);

