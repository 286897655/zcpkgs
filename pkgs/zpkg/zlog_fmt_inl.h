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

#ifndef ZPKG_ZLOG_FMT_INCLUDE_H_
#define ZPKG_ZLOG_FMT_INCLUDE_H_

#if __cplusplus >= 202207L
    #define ZLOG_USE_STD_FORMAT 1
#include <format>
#else
    #define ZLOG_USE_STD_FORAMT 0
#include <spdlog/fmt/fmt.h>
#endif

namespace zlog{

#if ZLOG_USE_STD_FORMAT > 0
using string_view_t = std::string_view;
using memory_buf_t = std::string;

template <typename... Args>
using using format_string_t = std::format_string<Args...>;
#else
using string_view_t = fmt::basic_string_view<char>;
using memory_buf_t = fmt::basic_memory_buffer<char, 256>;

template <typename... Args>
using format_string_t = fmt::format_string<Args...>;
#endif


};//!namespace zlog


#endif//!ZPKG_ZLOG_FMT_INCLUDE_H_