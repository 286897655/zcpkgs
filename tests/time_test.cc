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
 * @brief time test series
 */

#include <zlog/log.h>
#include <zpkg/times.h>

using namespace zpkg;

int main(int argc,char** argv){
    zlog::logger::create_defaultLogger();

    std::time_t now_time_t = std::time(NULL);
    // 直接打印应该比当前时间小8小时
    zlog("now_time_t fmt:{}",ctime::fmt_time_t(now_time_t));
    // 打印本地时间
    zlog("now local time:{}",times::fmt_now_s());

    // 通过gettimeofday获取秒和毫秒
    z_time_t now_get = times::system_clock_ms();// 也是utc标准时间
    time_t now_get_s = now_get / 1000;
    int now_get_ms = now_get % 1000;
    zlog("now_get_s fmt:{}",ctime::fmt_time_t(now_get_s));
    zlog("now_get_ms :{}",now_get_ms);
}