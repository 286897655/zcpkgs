#include "zpkg/clock.h"
#include <sys/time.h>
#include <time.h>

namespace zpkg{

z_time_t clock::now_ns(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    return ts.tv_sec * NSEC_PER_SEC + ts.tv_nsec;
}

z_time_t clock::now_us(){
    //  Use POSIX gettimeofday function to get precise time.
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return tv.tv_sec * USEC_PER_SEC + tv.tv_usec;
}

z_time_t clock::now_ms(){
    return now_us() / USEC_PER_MSEC;
}

};//!namespace zpkg