/** 
 * @copyright Copyright © 2020-2025 code by zhaoj
 * 
 * LICENSE
 * 
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

 /**
 * @author zhaoj 286897655@qq.com
 * @brief 
 */

#include "zpkg/times.h"
#include <sys/time.h>
#include <time.h>
#include <ctime>
#include <chrono>
#include "zcpkgs_common.h"

namespace zpkg{

/// Linux:monotonic_clock_gettime use clock_gettime(CLOCK_MONOTONIC) 
/// has ns precision,monotonic clock_gettime return tick from system start
/// Others:use chrono:steady_clock
static z_time_t monotonic_clock_gettime(){
#ifdef Z_SYS_LINUX
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    return ts.tv_sec * NSEC_PER_SEC + ts.tv_nsec;
#else
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
#endif
}

z_time_t times::steady_clock_ns(){
    return monotonic_clock_gettime();
}

z_time_t times::steady_clock_us(){
    return monotonic_clock_gettime() / NSEC_PER_USEC;
}

z_time_t times::steady_clock_ms(){
    return monotonic_clock_gettime() / NSEC_PER_MSEC;
}

/// Linux:gettimeofday_us use gettimeofday same as clock_gettime(CLOCK_REALTIME) 
/// has us precision,gettimeofday return time from UTC 1970-01-01 000000
/// Others:use chrono::system_clock

static z_time_t gettimeofday_us(){
#ifdef Z_SYS_LINUX
    timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * USEC_PER_SEC + now.tv_usec;
#else
    std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
#endif
}

z_time_t times::system_clock_us(){
    return gettimeofday_us();
}

z_time_t times::system_clock_ms(){
    return gettimeofday_us() / USEC_PER_MSEC;
}

z_time_t times::now_us(){
    return gettimeofday_us();
}

z_time_t times::now_ms(){
    return gettimeofday_us() / USEC_PER_MSEC;
}

// DateTime
std::string times::fmt_now_s(const char* fmt){
    time_t now = ::time(NULL);
    std::tm tm_snapshot;
#if defined(Z_SYS_WINDOWS)
    localtime_s(&tm_snapshot, &time); // thread-safe
#else
    localtime_r(&now, &tm_snapshot); // POSIX
#endif
    char buffer[1024];
    auto success = ::strftime(buffer, sizeof(buffer), fmt, &tm_snapshot);
    if (0 == success)
        return std::string(fmt);
    return buffer;
}

std::time_t ctime::utc_timet_now(){
    return ::time(NULL);
}

std::tm ctime::utc_tm_now(){
    std::time_t now = utc_timet_now();

    return timet2tm(now);
}
std::tm ctime::timet2tm(std::time_t time_tt){
    std::tm tm;
#ifdef _WIN32
    ::gmtime_s(&tm, &time_tt);
#else
    ::gmtime_r(&time_tt, &tm);
#endif
    return tm;
}

std::time_t ctime::tm2timet(std::tm* tm_tt){
    return ::timegm(tm_tt);
}

std::string ctime::fmt_timet(std::time_t time_tt,const char* fmt){
    std::tm tm_snapshot = timet2tm(time_tt);
    char buffer[1024];
    auto success = ::strftime(buffer, sizeof(buffer), fmt, &tm_snapshot);
    if (0 == success)
        return std::string(fmt);
    return buffer;
}

};//!namespace zpkg