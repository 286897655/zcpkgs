#pragma once

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
#include <ctime>
#include <chrono>

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

    // 通过gettimeofday获取utc时间,是utc标准时间无时区
    static z_time_t system_clock_us();
    static z_time_t system_clock_ms();

    static z_time_t now_us();
    static z_time_t now_ms();
    /**
     *   说明符	替换为	实例
     *   %a     缩写的星期几名称            Sun
     *   %A     完整的星期几名称            Sunday
     *   %b     缩写的月份名称              Mar
     *   %B     完整的月份名称              March
     *   %c     日期和时间表示法            Sun Aug 19 02:56:02 2012
     *   %d     一月中的第几天（01-31）     19
     *   %H     24 小时格式的小时（00-23）  14
     *   %I     12 小时格式的小时（01-12）  05
     *   %j     一年中的第几天（001-366）   231
     *   %m     十进制数表示的月份（01-12）  08
     *   %M     分（00-59）                55
     *   %p     AM 或 PM 名称              PM
     *   %S     秒（00-61）                02
     *   %U     一年中的第几周，以第一个星期日作为第一周的第一天（00-53）   33
     *   %w     十进制数表示的星期几，星期日表示为 0（0-6） 4
     *   %W     一年中的第几周，以第一个星期一作为第一周的第一天（00-53）   34
     *   %x     日期表示法                  08/19/12
     *   %X     时间表示法                  02:50:06
     *   %y     年份，最后两个数字（00-99）  01
     *   %Y     年份                        2012
     *   %Z     时区的名称或缩写             CDT
     *   %%     一个 % 符号                  %
     * 
     * samples:
     * "%Y-%m-%d %H:%M:%S" "1970-01-01 00:00:00"
     * "%Y-%m-%d-%H-%M-%S" "1970-01-01-00-00-00"
    */
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

/// @brief helper function for ctime
class ctime{
public:
    static std::time_t utc_timet();
    static std::tm timet2tm(std::time_t time_tt);
    static std::time_t tm2timet(std::tm* tm_tt);
    static std::string fmt_timet(std::time_t time_tt,const char* fmt = "%Y-%m-%d %H:%M:%S");
};

};//!namespace zpkg


#endif//!ZPKG_TIMES_H_