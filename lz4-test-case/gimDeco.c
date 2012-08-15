#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t Gubyte;
typedef uint16_t Gushort;
typedef uint32_t Guint;

int gimLz4Decompress(const unsigned char* input, unsigned char* output, size_t inSize)
{
    size_t Match_Len_Bits = 4;
    size_t Run_Bits = 8 - Match_Len_Bits;
    Guint Match_Len_Mask = (1 << Match_Len_Bits) - 1;
    Guint Run_Mask = (1 << Run_Bits) - 1;
    size_t Minimal_Match = 4;

    int copied = 0;

    while (1) {
        if (0 == inSize) {
            return -1;
        }
        Gubyte token = *input++;
        --inSize;

        Guint len = (token >> Match_Len_Bits);
        if (Run_Mask == len) {
            Gubyte lenContinued = 255;
            while (lenContinued == 255) {
                if (1 > inSize) { return -1; }
                lenContinued = *input++;
                --inSize;
                len += lenContinued;
            }
        }

        if (len > inSize) {
            return -1;
        }
        // copy literals
        memcpy(output, input, len);

        output += len;
        input += len;
        inSize -= len;
        copied += len;

        // specific to original implementation, end of stream is 5 literal bytes
        if (0 == inSize) {
            break; // !!! !!! !!!
        }

        //get offset
        if (2 > inSize) {
            fprintf(stderr, __FILE__ "ERROR: insize: %d, output: %d, len: %d\n", inSize, copied, len);
            return -1;
        }
        Gushort offset = *(Gushort*)input;
        input += 2;
        inSize -= 2;

        len = token & Match_Len_Mask;
        if (Match_Len_Mask == len) {
            Gubyte lenContinued = 255;
            while (lenContinued == 255) {
                if (1 > inSize) { return -1; }
                lenContinued = *input++;
                --inSize;
                len += lenContinued;
            }
        }

        if (offset > copied) {
            return -2;
        }

        int temp = (int)offset;
        if (offset == 0) {
            return -3;
        }
        len += Minimal_Match;

        if (offset > len) {
            memcpy(output, output - temp, len);

        } else {
            Gubyte* bOutSrc = output - temp;
            Gubyte* bOutDst = output;
            size_t count = len;
            if (offset == 1) {
                while (count--) {
                    *bOutDst++ = *bOutSrc++;
                }
            } else if (offset >= 2) {
                if (count % 2) {
                    *bOutDst++ = *bOutSrc++;
                    count--;
                }
                Gushort* wOutSrc = (Gushort*)bOutSrc;
                Gushort* wOutDst = (Gushort*)bOutDst;
                while (count) {
                    *wOutDst++ = *wOutSrc++;
                    count -= 2;
                }
            }
        }

        output += len;
        copied += len;
    }

    return copied;
}


