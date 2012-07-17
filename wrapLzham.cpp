#include "wrapLzham.h"

#include "lzham-read-only/include/lzham.h"

#include <cstdlib>
#include <cstdio>

using gim::pod::MemoryC;

int lzham_level = 4;
int lzham_flags = 0;

// apparently not implemented for *nixes
namespace lzham {
    Guint lzham_get_max_helper_threads() {
        return 0;
    }
}

unsigned int lzham_inflate(Gubyte** out, size_t* outSize, const MemoryC& input, const LodePNGDecompressSettings&)
{
    lzham_decompress_params params = { 0 };
    params.m_struct_size = sizeof(lzham_decompress_params);
    params.m_dict_size_log2 = 26;
    //params.m_decompress_flags |= LZHAM_DECOMP_FLAG_COMPUTE_ADLER32;
    params.m_decompress_flags |= LZHAM_DECOMP_FLAG_OUTPUT_UNBUFFERED;
    
    lzham_decompress_state_ptr decompState = lzham_decompress_init(&params);

    std::fprintf(stderr, "inflate ptr: %8p, %d in: %d\n", *out, *outSize, input.count);
    
    lzham_decompress_status_t status;
    size_t inBufSize = input.count - 4; // -4 due to appended CRC crap ;p
    status = lzham_decompress(decompState, input.ptr, &inBufSize, *out, outSize, 1);
    hexDump(MemoryC(*out, *outSize));
    if (LZHAM_DECOMP_STATUS_SUCCESS != status) {
        std::fprintf(stderr, "decompression error [%d]\n", status);
        return Error_General_Decompression;
    }
    std::fprintf(stderr, "inflate insize: %d outSize: %d\n", inBufSize, *outSize);
    return 0;
}

unsigned int lzham_deflate(Gubyte** out, size_t* outSize, const MemoryC& input, const LodePNGCompressSettings& settings)
{
    lzham_compress_params params = { 0 };
    params.m_struct_size = sizeof(lzham_compress_params);
    params.m_dict_size_log2 = 26; // 64 M
    params.m_max_helper_threads = 0;
    
    params.m_level = (lzham_compress_level)lzham_level;
    if (lzham_flags & Lzham_Polar)
        params.m_compress_flags |= LZHAM_COMP_FLAG_FORCE_POLAR_CODING;
    if (lzham_flags & Lzham_Deterministic)
        params.m_compress_flags |= LZHAM_COMP_FLAG_DETERMINISTIC_PARSING;
    //params.m_compress_flags |= LZHAM_COMP_FLAG_EXTREME_PARSING;
    //params.m_compress_flags |= LZHAM_COMP_FLAG_DETERMINISTIC_PARSING;
    //params.m_compress_flags |= LZHAM_COMP_FLAG_TRADEOFF_DECOMPRESSION_RATE_FOR_COMP_RATIO;
    params.m_cpucache_line_size = 0;
    params.m_cpucache_total_lines = 0;

    //lzham_compress_state_ptr compState = lzham_compress_init(&params);

    lzham_z_ulong worstCase = lzham_z_compressBound(input.count);
    if (0 == *out || *outSize < worstCase) {
        void* temp = std::realloc(*out, worstCase);
        if (0 == temp) {
            std::fprintf(stderr, "memory allocation problem\n");
            return Error_Memory_Allocation;
        }
        *out = static_cast<unsigned char*>( temp );
    }
    std::fprintf(stderr, "deflate ptr: %8p, %d in: %d\n", *out, *outSize, input.count);
    
    *outSize = worstCase;

    lzham_compress_status_t status;
    size_t inBufSize = input.count;
    hexDump(input);
    //status = lzham_compress(compState, input.ptr, &inBufSize, *out, outSize, 1);
    Guint adler = 0;
    status = lzham_compress_memory(&params, *out, outSize, input.ptr, inBufSize, &adler);
    if (LZHAM_COMP_STATUS_SUCCESS != status) {
        ::fprintf(stderr, "compression error [%d]\n", status);
        return Error_General_Compression;
    }
    std::fprintf(stderr, "deflate outSize: %d\n", *outSize);

    return 0;   
}

