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

#ifndef ZPKG_FUNCTIONS_H_
#define ZPKG_FUNCTIONS_H_

#include <functional>
#include <atomic>
#include <zpkg/utility.h>
#include <zpkg/times.h>

namespace zpkg{

using Func = std::function<void()>;

class TimeAferCall final: public apply_shared<TimeAferCall>{
public:
    TimeAferCall(z_time_t after_msec,std::function<bool(void)>&& boolean_cb);
    ~TimeAferCall() =default;

    z_time_t TimeAfter() const;
    bool Call() const;
    void Cancel();

private:
    z_time_t after_msec_;
    std::function<bool(void)> boolean_cb_;
    std::atomic_bool canceled_;
};

};//!namespace zpkg

#endif//!ZPKG_FUNCTIONS_H_