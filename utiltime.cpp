#include "utiltime.h"

#include <cstdio>
#include <sys/time.h>

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


