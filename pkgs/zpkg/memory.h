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

#ifndef ZPKG_MEMORY_HPP_
#define ZPKG_MEMORY_HPP_

#include <memory>

// linux <endian.h> defines
#if BYTE_ORDER == LITTLE_ENDIAN
#define Z_LITTLE_ENDIAN_HOST
#elif BYTE_ORDER == BIG_ENDIAN
#define Z_BIG_ENDIAN_HOST
#endif

/**
 * bswap 
 * byte swap routine
*/
#define Z_BSWAP16C(x) (((x) << 8 & 0xFF00)  | ((x) >> 8 & 0x00FF))
#define Z_BSWAP32C(x) (Z_BSWAP16C(x) << 16 | Z_BSWAP16C((x) >> 16))
#define Z_BSWAP64C(x) (Z_BSWAP32C(x) << 32 | Z_BSWAP32C((x) >> 32))
#define Z_BSWAPC(s, x) Z_BSWAP##s##C(x)

inline uint16_t z_bswap16_c(uint16_t x){
    x = (x>>8) | (x<<8);
    return x;
};

inline uint32_t z_bswap32_c(uint32_t x){
    return Z_BSWAP32C(x);
};

inline uint64_t z_bswap64_c(uint64_t x){
    return Z_BSWAP64C(x);
};

// be2he ... big-endian to host-endian
// le2he ... little-endian to host-endian

#ifdef Z_LITTLE_ENDIAN_HOST
#define z_be2he16(x) z_bswap16_c(x)
#define z_be2he32(x) z_bswap32_c(x)
#define z_be2he64(x) z_bswap64_c(x)
#define z_le2he16(x) (x)
#define z_le2he32(x) (x)
#define z_le2he64(x) (x)
#define Z_BE2HEC(s,x) Z_BSWAPC(s, x)
#define Z_LE2HEC(s,x) (x)
#elif defined Z_BIG_ENDIAN_HOST
#define z_be2he16(x) (x)
#define z_be2he32(x) (x)
#define z_be2he64(x) (x)
#define z_le2he16(x) z_bswap16_c(x)
#define z_le2he32(x) z_bswap32_c(x)
#define z_le2he64(x) z_bswap64_c(x)
#define Z_BE2HEC(s,x) (x)
#define Z_LE2HEC(s,x) Z_BSWAPC(s, x)
#endif

#define Z_BE2HE16C(x) Z_BE2HEC(16, x)
#define Z_BE2HE32C(x) Z_BE2HEC(32, x)
#define Z_BE2HE64C(x) Z_BE2HEC(64, x)
#define Z_LE2HE16C(x) Z_BE2HEC(16, x)
#define Z_LE2HE32C(x) Z_BE2HEC(32, x)
#define Z_LE2HE64C(x) Z_BE2HEC(64, x)


/*
* R/W means read/write, B/L means big/little/native endianness.
*/
#ifndef Z_RBE8
#define Z_RBE8(x) (((const uint8_t*)(x))[0])
#endif
#ifndef Z_RLE8
#define Z_RLE8(x) Z_RBE8(x)
#endif
#ifndef Z_RBE16
#   define Z_RBE16(x)                            \
    (((uint16_t)((const uint8_t*)(x))[0] << 8) |   \
                ((const uint8_t*)(x))[1])
#endif
#ifndef Z_WBE16
#   define Z_WBE16(p, val) do {               \
        uint16_t d = (val);                     \
        ((uint8_t*)(p))[1] = (d);               \
        ((uint8_t*)(p))[0] = (d)>>8;            \
    } while(0)
#endif
#ifndef Z_RLE16
#   define Z_RLE16(x)                           \
    (((uint16_t)((const uint8_t*)(x))[1] << 8) |  \
                ((const uint8_t*)(x))[0])
#endif
#ifndef Z_WLE16
#   define Z_WLE16(p, val) do {               \
        uint16_t d = (val);                     \
        ((uint8_t*)(p))[0] = (d);               \
        ((uint8_t*)(p))[1] = (d)>>8;            \
    } while(0)
#endif

#ifndef Z_RBE24
#   define Z_RBE24(x)                             \
    (((uint32_t)((const uint8_t*)(x))[0] << 16) |   \
               (((const uint8_t*)(x))[1] <<  8) |   \
                ((const uint8_t*)(x))[2])
#endif

#ifndef Z_WBE24
#   define Z_WBE24(p,val) do{                 \
        uint32_t d = (val);                     \
        ((uint8_t*)(p))[2] = (d);               \
        ((uint8_t*)(p))[1] = (d)>>8;            \
        ((uint8_t*)(p))[0] = (d)>>16;           \
    }while(0)
#endif

#ifndef Z_RLE24
#   define Z_RLE24(x)                             \
    (((uint32_t)((const uint8_t*)(x))[2] << 16) |   \
               (((const uint8_t*)(x))[1] <<  8) |   \
                ((const uint8_t*)(x))[0])
#endif

#ifndef Z_WLE24
#   define Z_WLE24(p, val) do {               \
        uint32_t d = (val);                     \
        ((uint8_t*)(p))[0] = (d);               \
        ((uint8_t*)(p))[1] = (d)>>8;            \
        ((uint8_t*)(p))[2] = (d)>>16;           \
    } while(0)
#endif

#ifndef Z_RBE32
#   define Z_RBE32(x)                             \
    (((uint32_t)((const uint8_t*)(x))[0] << 24) |   \
               (((const uint8_t*)(x))[1] << 16) |   \
               (((const uint8_t*)(x))[2] <<  8) |   \
                ((const uint8_t*)(x))[3])
#endif
#ifndef Z_WBE32
#   define Z_WBE32(p, val) do {               \
        uint32_t d = (val);                     \
        ((uint8_t*)(p))[3] = (d);               \
        ((uint8_t*)(p))[2] = (d)>>8;            \
        ((uint8_t*)(p))[1] = (d)>>16;           \
        ((uint8_t*)(p))[0] = (d)>>24;           \
    } while(0)
#endif
#ifndef Z_RLE32
#   define Z_RLE32(x)                              \
    (((uint32_t)((const uint8_t*)(x))[3] << 24) |    \
               (((const uint8_t*)(x))[2] << 16) |    \
               (((const uint8_t*)(x))[1] <<  8) |    \
                ((const uint8_t*)(x))[0])
#endif
#ifndef Z_WLE32
#   define Z_WLE32(p, val) do {               \
        uint32_t d = (val);                     \
        ((uint8_t*)(p))[0] = (d);               \
        ((uint8_t*)(p))[1] = (d)>>8;            \
        ((uint8_t*)(p))[2] = (d)>>16;           \
        ((uint8_t*)(p))[3] = (d)>>24;           \
    } while(0)
#endif
#ifndef Z_RBE64
#   define Z_RBE64(x)                                 \
    (((uint64_t)((const uint8_t*)(x))[0] << 56) |       \
     ((uint64_t)((const uint8_t*)(x))[1] << 48) |       \
     ((uint64_t)((const uint8_t*)(x))[2] << 40) |       \
     ((uint64_t)((const uint8_t*)(x))[3] << 32) |       \
     ((uint64_t)((const uint8_t*)(x))[4] << 24) |       \
     ((uint64_t)((const uint8_t*)(x))[5] << 16) |       \
     ((uint64_t)((const uint8_t*)(x))[6] <<  8) |       \
      (uint64_t)((const uint8_t*)(x))[7])
#endif
#ifndef Z_WBE64
#   define Z_WBE64(p, val) do {               \
        uint64_t d = (val);                     \
        ((uint8_t*)(p))[7] = (d);               \
        ((uint8_t*)(p))[6] = (d)>>8;            \
        ((uint8_t*)(p))[5] = (d)>>16;           \
        ((uint8_t*)(p))[4] = (d)>>24;           \
        ((uint8_t*)(p))[3] = (d)>>32;           \
        ((uint8_t*)(p))[2] = (d)>>40;           \
        ((uint8_t*)(p))[1] = (d)>>48;           \
        ((uint8_t*)(p))[0] = (d)>>56;           \
    } while(0)
#endif
#ifndef Z_RLE64
#   define Z_RLE64(x)                                 \
    (((uint64_t)((const uint8_t*)(x))[7] << 56) |       \
     ((uint64_t)((const uint8_t*)(x))[6] << 48) |       \
     ((uint64_t)((const uint8_t*)(x))[5] << 40) |       \
     ((uint64_t)((const uint8_t*)(x))[4] << 32) |       \
     ((uint64_t)((const uint8_t*)(x))[3] << 24) |       \
     ((uint64_t)((const uint8_t*)(x))[2] << 16) |       \
     ((uint64_t)((const uint8_t*)(x))[1] <<  8) |       \
      (uint64_t)((const uint8_t*)(x))[0])
#endif
#ifndef Z_WLE64
#   define Z_WLE64(p, val) do {               \
        uint64_t d = (val);                     \
        ((uint8_t*)(p))[0] = (d);               \
        ((uint8_t*)(p))[1] = (d)>>8;            \
        ((uint8_t*)(p))[2] = (d)>>16;           \
        ((uint8_t*)(p))[3] = (d)>>24;           \
        ((uint8_t*)(p))[4] = (d)>>32;           \
        ((uint8_t*)(p))[5] = (d)>>40;           \
        ((uint8_t*)(p))[6] = (d)>>48;           \
        ((uint8_t*)(p))[7] = (d)>>56;           \
    } while(0)
#endif


// make_unique support for pre c++14
#if __cplusplus <= 201402L && __cplusplus >= 201103L // C++14 and beyond
namespace std
{
// mock c++14 std::make_unique and enable_if_t
template <bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&...args) {
    static_assert(!std::is_array<T>::value, "arrays not supported");
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}//!namespace std
#endif

#endif//!ZPKG_MEMORY_HPP_