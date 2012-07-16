#include "lodepng.h"

#include "wrapLz4.h"
#include "wrapLzham.h"

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

void hexDump(const MemoryC& mem)
{
    /*
    FILE* fp = fopen("hexdump.bin", "w");
    for (size_t i = 0; i < mem.count; ++i) {
        std::fprintf(fp, "%02x ", mem.ptr[i]);
        if (!((i+1)%16)) {
            std::fprintf(fp, "\n");
        }
    }
    std::fclose(fp);
    */
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
    if (argc != 3) {
        std::cout << "syntax error\n";
    }
    std::cout << "converting..." << std::endl;

    gimDeflate = &lzham_deflate;
    gimInflate = &lzham_inflate;
    resave(argv[1], argv[2]);
    return 0;
}

