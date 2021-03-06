#include "wrapBsc.h"

#include "libbsc-0b12f29/libbsc/libbsc.h"

#include <cstdlib>
#include <cstdio>

using gim::pod::MemoryC;

int bsc_fast = 0;

unsigned int bsc_inflate(Gubyte** output, size_t* outSize, const MemoryC& input, const LodePNGDecompressSettings& settings)
{
    int features = bsc_fast ? LIBBSC_FEATURE_NONE : LIBBSC_FEATURE_FASTMODE;
    bsc_init(features);

    int blocksize = 0;
    int datasize = 0;
    bsc_block_info(input.ptr, LIBBSC_HEADER_SIZE, &blocksize, &datasize, features);

    //std::fprintf(stderr, "block: %d -> data: %d\n", blocksize, datasize);

    int res;
    // -4 due to crc at the end
    size_t inBufSize = input.count - 4;
    if (LIBBSC_NO_ERROR != (res = bsc_decompress(input.ptr, inBufSize, *output, *outSize, features))) {
        std::fprintf(stderr, "decompression error %d\n", res);
        return Error_General_Decompression;
    }

    *outSize = datasize;
    return 0;
}

unsigned int bsc_deflate(Gubyte** out, size_t* outSize, const MemoryC& input, const LodePNGCompressSettings& settings)
{
    size_t worstCase = input.count + LIBBSC_HEADER_SIZE;
    if (0 == *out || *outSize < worstCase) {
        void* temp = std::realloc(*out, worstCase);
        if (0 == temp) {
            std::fprintf(stderr, "memory allocation problem\n");
            return Error_Memory_Allocation;
        }
        *out = static_cast<unsigned char*>( temp );
    }
    //std::fprintf(stderr, "deflate ptr: %8p, %d in: %d\n", *out, *outSize, input.count);

    int features = bsc_fast ? LIBBSC_FEATURE_NONE : LIBBSC_FEATURE_FASTMODE;
    bsc_init(features);

    int res = bsc_compress(input.ptr, *out, input.count, 0, 0, LIBBSC_BLOCKSORTER_BWT, LIBBSC_CODER_QLFC_STATIC, features);
    if (res < 0) {
        std::fprintf(stderr, "compression error: %d\n", res);
        return Error_General_Compression;
    }

    *outSize = res;

    return 0;
}

