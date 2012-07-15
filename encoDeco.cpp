#include "lodepng.h"

#include "wrapLz4.h"

#include <iostream>

typedef unsigned (*Inflate)(unsigned char**, size_t*, const unsigned char*, size_t, const LodePNGDecompressSettings*);
typedef unsigned (*Deflate)(unsigned char**, size_t*, const unsigned char*, size_t, const LodePNGCompressSettings*);
 
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

