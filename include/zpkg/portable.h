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

#ifndef ZPKG_PORTABLE_H_
#define ZPKG_PORTABLE_H_

#if defined(NDEBUG) && defined(DEBUG)
#error "NDEBUG and DEBUG can't defined at same time"
#endif


#if (defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)) && !defined(_CRAYC)
#define Z_SYS_LINUX 1
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define Z_SYS_WINDOWS 1
#elif defined (__MACH__) && defined (__APPLE__)
#define Z_SYS_MACH 1
#endif

#if defined(Z_SYS_LINUX) && defined(Z_SYS_WINDOWS)
#error "Z_SYS_LINUX and Z_SYS_WINDOWS can't defined at same time"
#endif


#ifndef Z_INT_SUCCESS
#define Z_INT_SUCCESS 0
#endif

#ifndef Z_INT_FAIL
#define Z_INT_FAIL -1
#endif

#ifndef ZLikely
    #ifdef _MSC_VER
    #define ZLikely(cond) (cond)
    #elif defined(__GNUC__)
    #define ZLikely(cond) __builtin_expect(!!(cond), 1) 
    #endif
#endif

#ifndef ZUnlikely
    #ifdef _MSC_VER
    #define ZUnlikely(cond) (cond)
    #elif defined(__GNUC__)
    #define ZUnlikely(cond) __builtin_expect(!!(cond), 0) 
    #endif
#endif

#ifndef ZDELETE_P
#define ZDELETE_P(p) \
    if (p) { \
        delete p; \
        p = NULL; \
    } \
    (void)0
#endif

#ifndef ZDELETE_PA
#define ZDELETE_PA(pa) \
    if (pa) { \
        delete[] pa; \
        pa = NULL; \
    } \
    (void)0
#endif


//ASSERT
#ifdef NDEBUG // release
    #define Z_ASSERT(x) ((void)0)
#else
    #include <assert.h>
    #define Z_ASSERT(x) assert(x)
#endif

#endif //!ZPKG_PORTABLE_H_