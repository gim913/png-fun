#include "wrapDaSh.h"

#include "data-shrinker-read-only/Shrinker.h"

#include <cstdlib>
#include <cstdio>

using gim::pod::MemoryC;

unsigned int dash_inflate(Gubyte** output, size_t* outSize, const MemoryC& input, const LodePNGDecompressSettings& settings)
{
    // ugh, ugly const cast
    int res = shrinker_decompress(const_cast<Gubyte*>(input.ptr), *output, *outSize);
    if (res < 0) {
        std::fprintf(stderr, "decompression error %d\n", res);
        return Error_General_Decompression;
    }

    *outSize = res;

    return 0;
}

unsigned int dash_deflate(Gubyte** out, size_t* outSize, const MemoryC& input, const LodePNGCompressSettings& settings)
{
    // ugh, ugly const cast
    size_t worstCase = input.count + 128;
    if (0 == *out || *outSize < worstCase) {
        void* temp = std::realloc(*out, worstCase);
        if (0 == temp) {
            std::fprintf(stderr, "memory allocation problem\n");
            return Error_Memory_Allocation;
        }
        *out = static_cast<unsigned char*>( temp );
    }

    int res = shrinker_compress(const_cast<Gubyte*>(input.ptr), *out, input.count);
    if (res < 0) {
        std::fprintf(stderr, "compression error: %d\n", res);
        return Error_General_Compression;
    }

    *outSize = res;

    return 0;
}

