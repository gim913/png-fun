
#include "lodepng.h"

#include "lz4-read-only/lz4.h"
#include "lz4-read-only/lz4hc.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <sys/time.h>

void resave(const char* filename, const char* outName)
{
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;

  // load
  unsigned error = lodepng::decode(image, width, height, filename);
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //std::cout << "loaded..." << std::endl;

  // and save
  error = lodepng::encode(outName, image, width, height);
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;

  //std::cout << "saved..." << std::endl;
}

void* lodepng_timer_start()
{
    struct timeval* before = new struct timeval;
    gettimeofday(before, 0);
    return reinterpret_cast<void*>(before);
}

void lodepng_timer_stop(const char* prefix, void* timerStart)
{
    struct timeval after;
    struct timeval* before = reinterpret_cast<struct timeval*>(timerStart);
    gettimeofday(&after, 0);
    
    struct timeval* t = before;
    int diff_s = after.tv_sec - before->tv_sec;
    int diff_u = after.tv_usec - before->tv_usec;
    if (diff_u < 0) {
        diff_s -= 1;
        diff_u += 1000000;
    }
    ::fprintf(stderr, "%s load time ", prefix);
    if (diff_s)
        ::fprintf(stderr, "%d", diff_s);
    ::fprintf(stderr, "%06d us\n", diff_u);
}


typedef unsigned (*Inflate)(unsigned char**, size_t*, const unsigned char*, size_t, const LodePNGDecompressSettings*);
typedef unsigned (*Deflate)(unsigned char**, size_t*, const unsigned char*, size_t, const LodePNGCompressSettings*);
 
Inflate gimInflate;
Deflate gimDeflate;

unsigned lodepng_custom_inflate(
        unsigned char** outBufPtr,
        size_t* outSize,
        const unsigned char* inBuf,
        size_t inSize,
        const LodePNGDecompressSettings* settings)
{
    gimInflate(outBufPtr, outSize, inBuf, inSize, settings);
}
unsigned lodepng_custom_deflate(
        unsigned char** outBufPtr,
        size_t* outSize,
        const unsigned char* inBuf,
        size_t inSize,
        const LodePNGCompressSettings* settings)
{
    gimDeflate(outBufPtr, outSize, inBuf, inSize, settings);
}

unsigned lz4_inflate(unsigned char**, size_t*, const unsigned char*, size_t, const LodePNGDecompressSettings*);
unsigned lz4_deflate(unsigned char**, size_t*, const unsigned char*, size_t, const LodePNGCompressSettings*);

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cout << "syntax error\n";
    }
    std::cout << "converting..." << std::endl;
    gimDeflate = &lz4_deflate;
    gimInflate = &lz4_inflate;
    resave(argv[1], argv[2]);
    return 0;
}

typedef unsigned char        Gubyte;
typedef unsigned short       Gushort;
typedef unsigned int         Guint;
typedef unsigned long long   Gulong;
int gimLz4Decompress(const Gubyte* input, Gubyte* output, int inSize)
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
    while (true) {
        if (0 == inSize) {
            return -1;
        }
        Gubyte token = *input++;
        --inSize;

        Guint len = (token >> Match_Len_Bits);
        if (Run_Mask == len) {
            Gubyte lenContinued = 255;
            while (lenContinued == 255) {
                if (1 > inSize) { return -2; }
                lenContinued = *input++;
                --inSize;
                len += lenContinued;
            }
        }

        if (len > inSize) {
            return -3;
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
            return -4;
        }
        Gushort offset = *(Gushort*)input;
        input += 2;
        inSize -= 2;

        len = token & Match_Len_Mask;
        if (Match_Len_Mask == len) {
            Gubyte lenContinued = 255;
            while (lenContinued == 255) {
                if (1 > inSize) { return -5; }
                lenContinued = *input++;
                --inSize;
                len += lenContinued;
            }
        }

        if (offset > copied) {
            return -6;
        }

        int temp = (int)offset;
        if (offset == 0) {
            return -7;
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
    fprintf(stderr, "counters: %d %d %d\n", statCounter[0], statCounter[1], statCounter[2]);
#endif

    return copied;
}

void hexDump(const void* datav, size_t size)
{
    const Gubyte* data = static_cast<const Gubyte*>( datav );
    FILE *fp = fopen("hexdump.bin", "w");
    for (size_t i = 0; i < size; ++i) {
        fprintf (fp, "%02x ", data[i]);
        if (!((i+1)%4)) {
            fprintf (fp, " ");
        }
        if (!((i+1)%8)) {
            fprintf (fp, " ");
        }
        if (!((i+1)%16) || i == size-1) {
            fprintf (fp, "\n");
        }
    }
    fclose(fp);
}

unsigned lz4_inflate(unsigned char** out, size_t* outsize, const unsigned char* in, size_t inSize, const LodePNGDecompressSettings* settings)
{
    ::fprintf(stderr, "inflate ptr: %8p, %d in: %d\n", *out, *outsize, inSize);

    //const char* inMem = reinterpret_cast<const char*>(in);
    //char* outMem = reinterpret_cast<char*>(*out);
    // -4 due to appended CRC crap ;p
    //int res = LZ4_uncompress(inMem, outMem, inSize - 4);
    
    int res = gimLz4Decompress(in, *out, inSize - 4);
    if (res < 0) {
        ::fprintf(stderr, "decompression error %d\n", res);
        return -1;
    }
    Gubyte* tempBuf = *out;
    //hexDump(tempBuf, res);
    *outsize = res;
    return 0;
}

unsigned lz4_deflate(unsigned char** out, size_t* outSize, const unsigned char* in, size_t inSize, const LodePNGCompressSettings* settings)
{
    size_t worstCase = LZ4_compressBound(inSize);
    if (0 == *out || *outSize < worstCase) {
        void* temp = realloc(*out, worstCase);
        if (0 == temp) {
            ::fprintf(stderr, "memory allocation problem\n");
            return -1;
        }
        *out = static_cast<unsigned char*>( temp );
    }
    ::fprintf(stderr, "deflate ptr: %8p, %d in: %d\n", *out, *outSize, inSize);
    *outSize = 0;
    int res = LZ4_compressHC(reinterpret_cast<const char*>(in), reinterpret_cast<char*>(*out), inSize);
    if (res < 0) {
        ::fprintf(stderr, "compression error\n");
        return -1;
    }
    //hexDump(in, inSize);
    *outSize = res;
    ::fprintf(stderr, "deflate outSize: %d\n", res);

    return 0;
}
