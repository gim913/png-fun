#ifdef SELF_COMPILING_FILE
gcc lz4test.c gimDeco.c -o lz4test ../lz4-read-only/lz4hc.o ../lz4-read-only/lz4.o
md5sum --check test_files.md5
./lz4test testcase.bin test_lz4hc_output.bin
exit 0
#endif

#include "../lz4-read-only/lz4hc.h"
#include "../lz4-read-only/lz4.h"

#include <stdio.h>
#include <stdlib.h>

int gimLz4Decompress(const unsigned char* input, unsigned char* output, size_t inSize);

int main(int argc, char **argv)
{
    FILE* inFp;
    FILE* outFp;
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

    inFp = fopen(argv[1], "rb");
    outFp = fopen(argv[2], "wb");
    if (!inFp || !outFp) {
	fclose(inFp);
	fclose(outFp);
	fprintf (stderr, "can't open input: %s\n", argv[1]);
	return -2;
    }

    fseek(inFp, 0, SEEK_END);
    inSize = (size_t)ftell(inFp);
    fseek(inFp, 0, SEEK_SET);

    inMem = malloc(inSize);
    decoMem = calloc(inSize, 1);
    outMem = calloc(inSize*2, 1);
    if (!inMem || !outMem || !decoMem) {
	free(inMem);
	free(outMem);
	free(decoMem);
	fclose(inFp);
	fclose(outFp);
	fprintf (stderr, "oh noez, no mem!\n");
	return -3;
    }

    fread(inMem, 1, inSize, inFp);
    fclose(inFp);

    compRet = LZ4_compressHC(inMem, outMem, inSize);
    printf(" [+]   compressed %d to %d\n", inSize, compRet);
    fwrite(outMem, 1, compRet, outFp);

    decompRet = LZ4_uncompress(outMem, decoMem, inSize);
    printf(" [+] decompressed processed: %d\n", decompRet);

    decompRet = LZ4_uncompress_unknownOutputSize(outMem, decoMem, compRet, inSize);
    printf(" [+] decompressed %d from %d\n", decompRet, compRet);

    decompRet = gimLz4Decompress(outMem, decoMem, compRet);
    printf(" [+] gimlz4decomp %d from %d\n", decompRet, compRet);

    free(inMem);
    free(outMem);
    free(decoMem);
    fclose(outFp);

    return 0;
}
