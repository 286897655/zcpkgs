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

#ifndef ZCPKGS_COMMON_H_
#define ZCPKGS_COMMON_H_

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

#ifndef Z_Likely
    #ifdef _MSC_VER
    #define Z_Likely(cond) (cond)
    #elif defined(__GNUC__)
    #define Z_Likely(cond) __builtin_expect(!!(cond), 1) 
    #endif
#endif
#ifndef Z_Unlikely
    #ifdef _MSC_VER
    #define Z_Unlikely(cond) (cond)
    #elif defined(__GNUC__)
    #define Z_Unlikely(cond) __builtin_expect(!!(cond), 0) 
    #endif
#endif

// for old define
#ifndef ZUnlikely
    #define ZUnlikely Z_Unlikely
#endif

// Explicit declaration some variable
#ifndef Z_UNUSED
    #define Z_UNUSED(x) (void)x;
#endif

// min max
#define Z_MIN(a, b) (((a) < (b))? (a) : (b))
#define Z_MAX(a, b) (((a) < (b))? (b) : (a))

#ifdef __cplusplus
    extern "C"{
#endif
// assert override extern
extern void zpkg_assert(const char* assertion,const char* func,const char* file,int line);

extern void zpkg_assert_w(const char* what,const char* asssertion,const char* func,const char* file,int line);
#ifdef __cplusplus
    }
#endif

// assert define
#ifndef Z_ASSERT
    #ifdef NDEBUG 
        #define Z_ASSERT(cond) do { } while ((false) && (cond))
    #else
        #define Z_ASSERT(cond) ((!(cond)) ? zpkg_assert(#cond,__FUNCTION__,__FILE__,__LINE__) : ((void)0))
    #endif
#endif

#ifndef Z_ASSERT_W
    #ifdef NDEBUG 
        #define Z_ASSERT_W(cond,what) do { } while ((false) && (cond))
    #else
        #define Z_ASSERT_W(cond,what) ((!(cond)) ? zpkg_assert_w(what, #cond,__FUNCTION__,__FILE__,__LINE__) : ((void)0))
    #endif
#endif

#endif//!ZCPKGS_COMMON_H_