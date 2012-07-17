#pragma once

#include "wrappers.h"

unsigned int lz4_inflate(Gubyte**, size_t*, const gim::pod::MemoryC&, const LodePNGDecompressSettings&);
unsigned int lz4_deflate(Gubyte**, size_t*, const gim::pod::MemoryC&, const LodePNGCompressSettings&);

