#pragma once

#include "wrappers.h"

unsigned int snappy_inflate(Gubyte**, size_t*, const gim::pod::MemoryC&, const LodePNGDecompressSettings&);
unsigned int snappy_deflate(Gubyte**, size_t*, const gim::pod::MemoryC&, const LodePNGCompressSettings&);

