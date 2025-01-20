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

#ifndef ZPKG_TIMES_H_
#define ZPKG_TIMES_H_

#include <string>

using z_time_t = uint64_t;

// The time unit in ms, for example 100 * Z_UTIME_MILLISECONDS means 100ms.
#define Z_UTIME_MILLISECONDS 1000LL
// The time unit in second, for example 120 * Z_UTIME_SECONDS means 120s.
#define Z_UTIME_SECONDS 1000000LL

// The time unit in minutes, for example 3 * Z_UTIME_MINUTES means 3m.
#define Z_UTIME_MINUTES 60000000LL

// The time unit in hours, for example 2 * Z_UTIME_HOURS means 2h.
#define Z_UTIME_HOURS 3600000000LL

namespace zpkg{

enum{
    USEC_PER_MSEC = 1000LL,// 一毫秒1000微妙
    USEC_PER_SEC  = 1000000LL,// 一秒1000000微妙
    NSEC_PER_SEC  = 1000000000LL,// 一秒1000000000纳秒
    NSEC_PER_USEC = 1000LL // 一微秒1000纳秒
};

class times{
public:
    static z_time_t now_ns();
    static z_time_t now_us();
    static z_time_t now_ms();
    static std::string fmt_now_s(const char* fmt = "%Y-%m-%d %H:%M:%S");
};

};//!namespace zpkg


#endif//!ZPKG_TIMES_H_