/** 
 * @copyright Copyright © 2020-2024 code by zhaoj
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

#ifndef ZPKG_EXTERN_ASSERT_H_
#define ZPKG_EXTERN_ASSERT_H_

#include <stdio.h>

#ifndef NDEBUG
    #ifdef assert
        #undef assert
    #endif//assert

    #ifdef __cplusplus
    extern "C" {
    #endif
        extern void assert2throw(int failed, const char *exp, const char *func, const char *file, int line, const char *str);
    #ifdef __cplusplus
    }
    #endif

    #define assert(exp) assert2throw(!(exp), #exp, __FUNCTION__, __FILE__, __LINE__, NULL)
#else
    #define	assert(e)	((void)0)
#endif//NDEBUG

#endif//!ZPKG_EXTERN_ASSERT_H_