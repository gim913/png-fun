#include "wrapLzham.h"

#include "lzham-read-only/include/lzham.h"

using gim::pod::MemoryC;

unsigned int lzham_inflate(Gubyte**, size_t*, const gim::pod::MemoryC&, const LodePNGDecompressSettings&)
{

}

unsigned int lzham_deflate(Gubyte**, size_t*, const gim::pod::MemoryC&, const LodePNGCompressSettings&)\
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
    lzham_compress_status_t status = LZHAM_COMP_STATUS_FAILED;
}

