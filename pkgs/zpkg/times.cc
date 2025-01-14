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

namespace zpkg{


z_time_t times::now_ns(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC,&ts);
    return ts.tv_sec * NSEC_PER_SEC + ts.tv_nsec;
}

z_time_t times::now_us(){
    //  Use POSIX gettimeofday function to get precise time.
    struct timeval tv;
    gettimeofday (&tv, NULL);
    return tv.tv_sec * USEC_PER_SEC + tv.tv_usec;
}

z_time_t times::now_ms(){
    return now_us() / USEC_PER_MSEC;
}

// DateTime
std::string times::fmt_now_s(const char* fmt)
{
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

};//!namespace zpkg