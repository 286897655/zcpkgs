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
    NSEC_PER_USEC = 1000LL,// 1微妙1000纳秒
    USEC_PER_MSEC = 1000LL,// 1毫秒1000微妙
    MSEC_PER_SEC  = 1000LL,// 一秒1000毫秒
    USEC_PER_SEC  = USEC_PER_MSEC * MSEC_PER_SEC,// 1秒1000000微妙
    NSEC_PER_SEC  = NSEC_PER_USEC * USEC_PER_SEC,// 1秒1000000000纳秒
    NSEC_PER_MSEC = NSEC_PER_USEC * USEC_PER_MSEC, // 1毫秒1000000纳秒
};

class times{
public:
    static z_time_t steady_clock_ns();
    static z_time_t steady_clock_us();
    static z_time_t steady_clock_ms();

    static z_time_t system_clock_us();
    static z_time_t system_clock_ms();

    static z_time_t now_us();
    static z_time_t now_ms();
    static std::string fmt_now_s(const char* fmt = "%Y-%m-%d %H:%M:%S");
};

class ticker_ms{
public:
    ticker_ms(){
        created_ = begin_ = times::steady_clock_ms();
    }
    z_time_t elapse() const{
        return times::steady_clock_ms() - begin_;
    }
    z_time_t created() const{
        return times::steady_clock_ms() - created_;
    }
    void reset_elapse_tick(){
        begin_ = times::steady_clock_ms();
    }

private:
    z_time_t created_;
    z_time_t begin_;
};

};//!namespace zpkg


#endif//!ZPKG_TIMES_H_