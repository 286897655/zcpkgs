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

#ifndef ZPKG_BUFFER_HPP_
#define ZPKG_BUFFER_HPP_
#include <stddef.h>
#include <stdint.h>


namespace zpkg{

/**
 * bytes:1 2 3 4 5 6 7 8
 * cross one byte
 *       2 1 4 3 6 5 8 7
 * usefule in int16_t le<->be in simd version
 * 
 * caller assume size = 2*x
 * 
 * default in simd version
 * and have c 
 */
void cross_byte_u8(const uint8_t* bytes,size_t size);
void cross_byte_u8_c(const uint8_t* bytes,size_t size);
void cross_byte_s16(const int16_t* bytes,size_t size);
void cross_byte_s16_c(const int16_t* bytes,size_t size);

class bit_buffer{

};

}//!namespace zpkg


#endif //!ZPKG_BUFFER_HPP_