#include "wrapSnappy.h"

#include "snappy-read-only/snappy.h"

#include <cstdlib>
#include <cstdio>

using gim::pod::MemoryC;

unsigned int snappy_inflate(Gubyte** output, size_t* outSize, const MemoryC& input, const LodePNGDecompressSettings& settings)
{
    // -4 due to crc at the end
    size_t inBufSize = input.count - 4;
    bool res = snappy::GetUncompressedLength(reinterpret_cast<const char*>(input.ptr), inBufSize, outSize);
    if (!res) {
        std::fprintf(stderr, "upps, huston we might have a problem\n");
        return Error_General_Decompression;
    }

    res = snappy::RawUncompress(
            reinterpret_cast<const char*>(input.ptr), inBufSize,
            reinterpret_cast<char*>(*output));
    if (!res) {
        std::fprintf(stderr, "decompression error %d\n", res);
        return Error_General_Decompression;
    }

    return 0;
}

unsigned int snappy_deflate(Gubyte** out, size_t* outSize, const MemoryC& input, const LodePNGCompressSettings& settings)
{
    // ugh, ugly const cast
    size_t worstCase = snappy::MaxCompressedLength(input.count);
    if (0 == *out || *outSize < worstCase) {
        void* temp = std::realloc(*out, worstCase);
        if (0 == temp) {
            std::fprintf(stderr, "memory allocation problem\n");
            return Error_Memory_Allocation;
        }
        *out = static_cast<unsigned char*>( temp );
    }

    // hmm this one is 'void' so i guess it can throw... ?
    *outSize = worstCase;
    snappy::RawCompress(
            reinterpret_cast<const char*>(input.ptr), input.count,
            reinterpret_cast<char*>(*out), outSize);

    return 0;
}


