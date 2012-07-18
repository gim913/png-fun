#include "utiltime.h"

#include <cstdio>
#include <sys/time.h>

void* lodepng_timer_start()
{
    struct timeval* before = new struct timeval;
    gettimeofday(before, 0);
    return reinterpret_cast<void*>(before);
}

void lodepng_timer_stop(const char* prefix, void* timerStart, size_t size1, size_t size2)
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
    std::fprintf(stderr, "%s time ", prefix);
    if (diff_s) {
        std::fprintf(stderr, "%d", diff_s);
        std::fprintf(stderr, "%06d us\n", diff_u);

    } else {
        std::fprintf(stderr, "%d us\n", diff_u);
    }

    std::fprintf(stderr, "%s data %zd -> %zd\n", prefix, size1, size2);
}


