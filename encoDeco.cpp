#include "lodepng.h"

#include "wrapLz4.h"
#include "wrapLzham.h"
#include "wrapBsc.h"
#include "wrapDaSh.h"
#include "wrapSnappy.h"

#include <iostream>

using gim::pod::MemoryC;

typedef unsigned (*Inflate)(Gubyte**, size_t*, const MemoryC&, const LodePNGDecompressSettings&);
typedef unsigned (*Deflate)(Gubyte**, size_t*, const MemoryC&, const LodePNGCompressSettings&);
 
Inflate gimInflate;
Deflate gimDeflate;

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

#include <cstdio>

int do_dump = 0;
void hexDump(const MemoryC& mem)
{
    if (!do_dump)
        return;

    FILE* fp = std::fopen("hexdump.bin", "w");
    for (size_t i = 0; i < mem.count; ++i) {
        std::fprintf(fp, "%02x ", mem.ptr[i]);
        if (!((i+1)%16)) {
            std::fprintf(fp, "\n");
        }
    }
    std::fclose(fp);
}

unsigned lodepng_custom_inflate(
        unsigned char** outBufPtr,
        size_t* outSize,
        const unsigned char* inBuf,
        size_t inSize,
        const LodePNGDecompressSettings* settings)
{
    gimInflate(outBufPtr, outSize, MemoryC(inBuf, inSize), *settings);
}
unsigned lodepng_custom_deflate(
        unsigned char** outBufPtr,
        size_t* outSize,
        const unsigned char* inBuf,
        size_t inSize,
        const LodePNGCompressSettings* settings)
{
    gimDeflate(outBufPtr, outSize, MemoryC(inBuf, inSize), *settings);
}

int main(int argc, char **argv)
{
    if (argc < 4) {
        std::cout << "syntax error\n";
        std::cout << argv[0] << " [codec] input output\n";
        std::cout << "where [codec] is on of the following:\n";
        std::cout << "\t4 - to use lz4 algorithm\n";
        std::cout << "\th - to use lzham algorithm, can be followed by:\n";
        std::cout << "\t\tnumber 0-4 - compression level\n";
        std::cout << "\t\td|p|dp - deterministic compression, polar coordinates, both\n";
        std::cout << "\t\t\te.g: h4dp\n";
        std::cout << "\tb - to use bsc algorithm, can be followed by:\n";
        std::cout << "\t\tf - fast mode in bsc features flags\n";
        return -1;
    }
    if (argc > 4) do_dump = 1;
    std::cout << "will " << (do_dump?"":"NOT ") << "be doing debug hexdump\n";

    std::cout << "converting algo:";


    // i'm too lazy to use getopt, 
    switch (argv[1][0]) {
        case '4':
            gimDeflate = &lz4_deflate;
            gimInflate = &lz4_inflate;
            std::cout << "lz4" << std::endl;
            break;

        case 'h':
            gimDeflate = &lzham_deflate;
            gimInflate = &lzham_inflate;
            std::cout << "lzham" << std::endl;

            if (strlen(argv[1]) > 1 && std::isdigit(argv[1][1])) {
                lzham_level =  argv[1][1] - '0';
            }
            std::cout << "lzham using compression level: " << lzham_level << std::endl;
            if (strlen(argv[1]) > 2) {
                if ('p' == argv[1][2]) {
                    lzham_flags |= Lzham_Polar;
                } else if ('d' == argv[1][2]) {
                    lzham_flags |= Lzham_Deterministic;
                }
            }
            if (strlen(argv[1]) > 3) {
                if ('p' == argv[1][3]) {
                    lzham_flags |= Lzham_Polar;
                } else if ('d' == argv[1][3]) {
                    lzham_flags |= Lzham_Deterministic;
                }
            }
            std::cout << "lzham " << ((lzham_flags&Lzham_Polar)?"":"NOT ") << "using polar coordinates" << std::endl;
            std::cout << "lzham " << ((lzham_flags&Lzham_Deterministic)?"":"NOT ") << "using deterministic compression" << std::endl;
            break;
        case 'd':
            gimInflate = &dash_inflate;
            gimDeflate = &dash_deflate;
            std::cout << "dash" << std::endl;
            break;
        case 's':
            gimInflate = &snappy_inflate;
            gimDeflate = &snappy_deflate;
            std::cout << "snappy" << std::endl;
            break;
        case 'b':
            gimDeflate = &bsc_deflate;
            gimInflate = &bsc_inflate;
            std::cout << "bsc" << std::endl;
            if (strlen(argv[1]) > 1) {
                if (argv[1][1] == 'f') {
                    bsc_fast = 1;
                }
            }
            std::cout << "bsc " << (bsc_fast?"":"NOT ") << "using fast mode" << std::endl;
            break;

        default:
            std::cout << "uknown value in first argument" << std::endl;
            return -1;
    }

    resave(argv[2], argv[3]);
    return 0;
}

