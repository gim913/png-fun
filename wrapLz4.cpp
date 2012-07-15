#include "wrapLz4.h"

#include "lz4-read-only/lz4.h"
#include "lz4-read-only/lz4hc.h"

#include <cstdio>
#include <cstdlib>


using gim::pod::MemoryC;

// I had a problem when using original LZ4_uncompress...
int gimLz4Decompress(const MemoryC& _input, Gubyte* output)
{
    static const size_t Match_Len_Bits = 4;
    static const size_t Run_Bits = 8 - Match_Len_Bits;
    static const Guint Match_Len_Mask = (1 << Match_Len_Bits) - 1;
    static const Guint Run_Mask = (1 << Run_Bits) - 1;
    static const size_t Minimal_Match = 4;

    int copied = 0;

#ifdef LZ4_DEBUG_COUNTERS
    int statCounter[3] = { 0 };
#define LZ4COUNTER(x) statCounter[x]++
#else
#define LZ4COUNTER(x)
#endif

    size_t inSize = _input.count;
    MemoryC::ValueType* input = _input.ptr;
    while (true) {
        if (0 == inSize) {
            return -Error_Premature_End_Of_Data;
        }
        Gubyte token = *input++;
        --inSize;

        Guint len = (token >> Match_Len_Bits);
        if (Run_Mask == len) {
            Gubyte lenContinued = 255;
            while (lenContinued == 255) {
                if (1 > inSize) { return -Error_Premature_End_Of_Data; }
                lenContinued = *input++;
                --inSize;
                len += lenContinued;
            }
        }

        if (len > inSize) {
            return -Error_Premature_End_Of_Data;
        }
        // copy literals
        memcpy(output, input, len);

        output += len;
        input += len;
        inSize -= len;
        copied += len;

        // specific to original implementation, end of stream is 5 literal bytes
        if (0 == inSize) {
            ::fprintf(stderr, "%d bytes written, quitting, due to lack of more data\n", copied);
            break; // !!! !!! !!!
        }

        //get offset
        if (2 > inSize) {
            ::fprintf(stderr, "insize: %d, output: %d, len: %d\n", inSize, copied, len);
            return -Error_Premature_End_Of_Data;
        }
        Gushort offset = *(Gushort*)input;
        input += 2;
        inSize -= 2;

        len = token & Match_Len_Mask;
        if (Match_Len_Mask == len) {
            Gubyte lenContinued = 255;
            while (lenContinued == 255) {
                if (1 > inSize) { return -Error_Premature_End_Of_Data; }
                lenContinued = *input++;
                --inSize;
                len += lenContinued;
            }
        }

        if (offset > copied) {
            return -Error_Invalid_Distance;
        }

        int temp = (int)offset;
        if (offset == 0) {
            return -Error_Invalid_Distance;
        }
        len += Minimal_Match;

        if (offset > len) {
            LZ4COUNTER(0);
            memcpy(output, output - temp, len);

        } else {
            Gubyte* bOutSrc = output - temp;
            Gubyte* bOutDst = output;
            size_t count = len;
            if (offset == 1) {
                LZ4COUNTER(1);
                while (count--) {
                    *bOutDst++ = *bOutSrc++;
                }
            } else if (offset >= 2) {
                LZ4COUNTER(2);
                if (count % 2) {
                    *bOutDst++ = *bOutSrc++;
                    count--;
                }
                Gushort* wOutSrc = reinterpret_cast<Gushort*>(bOutSrc);
                Gushort* wOutDst = reinterpret_cast<Gushort*>(bOutDst);
                while (count) {
                    *wOutDst++ = *wOutSrc++;
                    count -= 2;
                }
            }
        }

        output += len;
        copied += len;
    }
#ifdef LZ4_DEBUG_COUNTERS
    ::fprintf(stderr, "counters: %d %d %d\n", statCounter[0], statCounter[1], statCounter[2]);
#endif

    return copied;
}


unsigned int lz4_inflate(Gubyte** out, size_t* outSize, const MemoryC& input, const LodePNGDecompressSettings& settings)
{
    ::fprintf(stderr, "inflate ptr: %8p, %d in: %d\n", *out, *outSize, input.count);
    
    // -4 due to appended CRC crap ;p
    //int res = LZ4_uncompress(inMem, outMem, inSize - 4);
    int res = gimLz4Decompress(MemoryC(input.ptr, input.count - 4), *out);
    if (res < 0) {
        ::fprintf(stderr, "decompression error %d\n", res);
        return -res;
    }
    *outSize = res;
    return 0;
}

unsigned int lz4_deflate(Gubyte** out, size_t* outSize, const MemoryC& input, const LodePNGCompressSettings& settings)
{
    size_t worstCase = LZ4_compressBound(input.count);
    if (0 == *out || *outSize < worstCase) {
        void* temp = ::realloc(*out, worstCase);
        if (0 == temp) {
            ::fprintf(stderr, "memory allocation problem\n");
            return Error_Memory_Allocation;
        }
        *out = static_cast<unsigned char*>( temp );
    }
    ::fprintf(stderr, "deflate ptr: %8p, %d in: %d\n", *out, *outSize, input.count);
    *outSize = 0;
    int res = LZ4_compressHC(input.as<const char>().ptr, reinterpret_cast<char*>(*out), input.count);
    if (res < 0) {
        ::fprintf(stderr, "compression error\n");
        return Error_Compression;
    }
    *outSize = res;
    ::fprintf(stderr, "deflate outSize: %d\n", res);

    return 0;
}
