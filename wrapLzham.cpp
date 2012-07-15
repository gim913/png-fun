#include "wrapLzham.h"

#include "lzham-read-only/include/lzham.h"

#include <cstdio>

using gim::pod::MemoryC;

namespace lzham {
    Guint lzham_get_max_helper_threads() {
        return 0;
    }
}


unsigned int lzham_inflate(Gubyte** out, size_t* outSize, const gim::pod::MemoryC& input, const LodePNGDecompressSettings&)
{
    lzham_decompress_params params = { 0 };
    params.m_struct_size = sizeof(lzham_decompress_params);
    params.m_dict_size_log2 = 26;
    //params.m_decompress_flags |= LZHAM_DECOMP_FLAG_COMPUTE_ADLER32;
    
    lzham_decompress_state_ptr decompState = lzham_decompress_init(&params);

    ::fprintf(stderr, "inflate ptr: %8p, %d in: %d\n", *out, *outSize, input.count);
    
    lzham_decompress_status_t status;
    size_t inBufSize = input.count - 4; // -4 due to appended CRC crap ;p
    status = lzham_decompress(decompState, input.ptr, &inBufSize, *out, outSize, 1);
    //if (res < 0) {
    //    ::fprintf(stderr, "decompression error %d\n", res);
    //    return -res;
    //}
    ::fprintf(stderr, "inflate insize: %d outSize: %d [%d]\n", inBufSize, *outSize, status);
    return 0;
}

unsigned int lzham_deflate(Gubyte** out, size_t* outSize, const gim::pod::MemoryC& input, const LodePNGCompressSettings& settings)
{
    lzham_compress_params params = { 0 };
    params.m_struct_size = sizeof(lzham_compress_params);
    params.m_dict_size_log2 = 26; // 64 M
    params.m_max_helper_threads = 0;
    params.m_level = LZHAM_COMP_LEVEL_FASTEST;
    params.m_compress_flags |= LZHAM_COMP_FLAG_FORCE_POLAR_CODING;
    //params.m_compress_flags |= LZHAM_COMP_FLAG_EXTREME_PARSING;
    params.m_compress_flags |= LZHAM_COMP_FLAG_DETERMINISTIC_PARSING;
    //params.m_compress_flags |= LZHAM_COMP_FLAG_TRADEOFF_DECOMPRESSION_RATE_FOR_COMP_RATIO;
    params.m_cpucache_line_size = 0;
    params.m_cpucache_total_lines = 0;

    lzham_compress_state_ptr compState = lzham_compress_init(&params);

    lzham_z_ulong worstCase = lzham_z_compressBound(input.count);
    if (0 == *out || *outSize < worstCase) {
        void* temp = ::realloc(*out, worstCase);
        if (0 == temp) {
            ::fprintf(stderr, "memory allocation problem\n");
            return Error_Memory_Allocation;
        }
        *out = static_cast<unsigned char*>( temp );
    }
    ::fprintf(stderr, "deflate ptr: %8p, %d in: %d\n", *out, *outSize, input.count);
    
    *outSize = worstCase;

    lzham_compress_status_t status;
    size_t inBufSize = input.count;
    status = lzham_compress(compState, input.ptr, &inBufSize, *out, outSize, 1);
    //if (res < 0) {
    //    ::fprintf(stderr, "compression error\n");
    //    return Error_Compression;
    //}
    ::fprintf(stderr, "deflate outSize: %d [%d]\n", *outSize, status);

    return 0;   
}

