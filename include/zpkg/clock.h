#ifndef ZPKG_CLOCK_H_
#define ZPKG_CLOCK_H_

#include <stdint.h>
#include <stddef.h>
namespace zpkg{

using z_time_t = uint64_t;

// The time unit in ms, for example 100 * Z_UTIME_MILLISECONDS means 100ms.
#define Z_UTIME_MILLISECONDS 1000LL
// The time unit in second, for example 120 * Z_UTIME_SECONDS means 120s.
#define Z_UTIME_SECONDS 1000000LL

// The time unit in minutes, for example 3 * Z_UTIME_MINUTES means 3m.
#define Z_UTIME_MINUTES 60000000LL

// The time unit in hours, for example 2 * Z_UTIME_HOURS means 2h.
#define Z_UTIME_HOURS 3600000000LL

class clock{
public:
    enum{
        USEC_PER_MSEC = 1000LL,// 一毫秒1000微妙
        USEC_PER_SEC  = 1000000LL,// 一秒1000000微妙
        NSEC_PER_SEC  = 1000000000LL,// 一秒1000000000纳秒
        NSEC_PER_USEC = 1000LL // 一微秒1000纳秒
    };
    static z_time_t now_ns();
    static z_time_t now_us();
    static z_time_t now_ms();
};

};//!namespace zpkg

#endif//!ZPKG_CLOCK_H_