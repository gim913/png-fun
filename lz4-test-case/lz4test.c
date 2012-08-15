#ifdef SELF_COMPILING_FILE
gcc lz4test.c gimDeco.c -o lz4test ../lz4-read-only/lz4hc.o ../lz4-read-only/lz4.o
exit 1
#endif

#include "../lz4-read-only/lz4hc.h"
#include "../lz4-read-only/lz4.h"

#include <stdio.h>
#include <stdlib.h>

int gimLz4Decompress(const unsigned char* input, unsigned char* output, size_t inSize);

int main(int argc, char **argv)
{
    FILE* fp;
    size_t inSize;
    void* inMem;
    void* outMem;
    void* decoMem;
    int compRet = 0;
    int decompRet = 0;

    if (argc != 3) {
	fprintf (stderr, "oh noez!\n");
	return -1;
    }

    fp = fopen(argv[1], "rb");
    if (!fp) {
	fprintf (stderr, "can't open input: %s\n", argv[1]);
	return -2;
    }

    fseek(fp, 0, SEEK_END);
    inSize = (size_t)ftell(fp);
    fseek(fp, 0, SEEK_SET);

    inMem = malloc(inSize);
    decoMem = calloc(inSize, 1);
    outMem = calloc(inSize*2, 1);
    if (!inMem || !outMem || !decoMem) {
	free(inMem);
	free(outMem);
	free(decoMem);
	fprintf (stderr, "oh noez, no mem!\n");
	return -3;
    }

    fread(inMem, 1, inSize, fp);
    fclose(fp);

    compRet = LZ4_compressHC(inMem, outMem, inSize);
    printf(" [+]   compressed %d to %d\n", inSize, compRet);

    decompRet = LZ4_uncompress(outMem, decoMem, compRet);
    printf(" [+] decompressed %d from %d\n", decompRet, compRet);

    decompRet = gimLz4Decompress(outMem, decoMem, compRet);
    printf(" [+] gimlz4decomp %d from %d\n", decompRet, compRet);

    free(inMem);
    free(outMem);
    free(decoMem);

    return 0;
}
