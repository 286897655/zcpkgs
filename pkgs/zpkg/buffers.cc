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

#include "zpkg/buffers.h"
#include <cstring>
#ifdef __x86_64__
#include <immintrin.h>
#elif __ARM_NEON
#include <arm_neon.h>
#endif
#include "zpkg/memory.h"
#include "zpkg/utility.h"

namespace zpkg{

void cross_byte_u8_c(const uint8_t* buffer,size_t size)
{
    for(size_t i = 0; i < size; ){
        uint16_t read = Z_RBE16(buffer + i);
        Z_WLE16(buffer + i,read);

        // cross 2 bytes each time
        i+=2;
    }
};

#ifdef __SSSE3__
static void cross_byte_u8_x86_sse(const uint8_t* buffer,size_t size)
{
    constexpr static size_t CROSS_BYTE = 128 / 8;
    constexpr static size_t LOG2_CROSS_BYTE = 4;
    size_t count = size >> LOG2_CROSS_BYTE;
    size_t mod = size & (CROSS_BYTE - 1);
    for(size_t i = 0 ;i < count ; i++){
        uint8_t* pos = (uint8_t*)buffer + i * CROSS_BYTE;
        // x86 load in little endian
        const static __m128i ShuffleRev = _mm_setr_epi8
        (
            1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14
        );
        // Load 16 bytes at once into one 16-byte register
        __m128i load = _mm_loadu_si128(
            reinterpret_cast<__m128i*>(pos)
        );
        __m128i cross = _mm_shuffle_epi8(load,ShuffleRev);
        _mm_storeu_si128(reinterpret_cast<__m128i*>(pos),cross);
    }
    // un 16bytes aligned use c code
    cross_byte_u8_c(buffer + CROSS_BYTE * count,mod);
};
#endif

#ifdef __ARM_NEON
static void cross_byte_u8_arm_neon(const uint8_t* buffer,size_t size){
    constexpr static size_t CROSS_BYTE = 128 / 8;
    constexpr static size_t LOG2_CROSS_BYTE = 4;
    size_t count = size >> LOG2_CROSS_BYTE;
    size_t mod = size & (CROSS_BYTE - 1);
    for(size_t i = 0 ;i < count ; i++){
        uint8_t* pos = (uint8_t*)buffer + i * CROSS_BYTE;
        // use in tbl (table loopup)
        // static constexpr uint8_t data[16] = {1,  0,  3,  2,
        //                                      5,  4,  7,  6,
        //                                      9,  8, 11, 10,
        //                                     13, 12, 15, 14};
        // // load in little endian
        // const static uint8x16_t ShuffleRev = vld1q_u8(data);
        // // Load 16 bytes at once into one 16-byte register
        // uint8x16_t load = vld1q_u8(pos);
        // uint8x16_t cross = vqtbl1q_u8(load,ShuffleRev);
        // vst1q_u8(pos,cross);
        // use in reverse
        vst1q_u8(pos,vrev16q_u8(vld1q_u8(pos)));
    }
    // un 16bytes aligned use c code
    cross_byte_u8_c(buffer + CROSS_BYTE * count,mod);
};
#endif

void cross_byte_u8(const uint8_t* buffer,size_t size)
{
    Z_ASSERT(!(size & 0x01));
    #ifdef __SSSE3__
    cross_byte_u8_x86_sse(buffer,size);
    #elif __ARM_NEON
    cross_byte_u8_arm_neon(buffer,size);
    #else
    cross_byte_u8_c(buffer,size);
    #endif
}

void cross_byte_s16_c(const int16_t* bytes,size_t size)
{
    cross_byte_u8_c((const uint8_t*)bytes,size * sizeof(int16_t));
};
void cross_byte_s16(const int16_t* bytes,size_t size)
{
    #ifdef __SSSE3__
    cross_byte_u8_x86_sse((const uint8_t*)bytes,size * sizeof(int16_t));
    #elif __ARM_NEON
    cross_byte_u8_arm_neon((const uint8_t*)bytes,size * sizeof(int16_t));
    #else
    cross_byte_u8_c((const uint8_t*)bytes,size * sizeof(int16_t));
    #endif
}
//////////////////////////////////byte_stream/////////////////////////////////////////////////////
byte_stream::byte_stream(uint8_t* bytes,size_t sizeBytes):start_(bytes),end_(bytes + sizeBytes),pos_(bytes){

}
bool byte_stream::eof() const{
    return pos_ >= end_;
}

uint8_t* byte_stream::head() const{
    return start_;
}

size_t byte_stream::size() const{
    return end_ - start_;
}

uint8_t* byte_stream::pos() const{
    return pos_;
}

size_t byte_stream::left() const{
    return end_ - pos_;
}

size_t byte_stream::lapped() const{
    return pos_ - start_;
}

size_t byte_stream::peek(uint8_t* out,size_t sizeOut){
    if(eof()){
        return 0;
    }

    size_t leftbuffer = left();
    if(leftbuffer < sizeOut){
        // has not enough bytes
        sizeOut = leftbuffer;
    }
    std::memcpy(out,pos_,sizeOut);
    return sizeOut;
}

size_t byte_stream::read(uint8_t* out,size_t sizeOut){
    if(eof()){
        return 0;
    }
    size_t leftbuffer = left();
    if(leftbuffer < sizeOut){
        // has not enough bytes
        sizeOut = leftbuffer;
    }
    std::memcpy(out,pos_,sizeOut);
    pos_ += sizeOut;
    return sizeOut;
}

size_t byte_stream::write(const uint8_t* in,size_t sizeIn){
    if(eof()){
        return 0;
    }
    size_t leftbuffer = left();
    if(leftbuffer < sizeIn){
        // has not enough bytes
        sizeIn = leftbuffer;
    }
    std::memcpy(pos_,in,sizeIn);
    pos_ += sizeIn;
    return sizeIn;
}

bool byte_stream::require(size_t requiredSize) const{
    return requiredSize <= left();
};

int byte_stream::skip(int step){
    if(step > 0){
        size_t leftbuffer = left();
        if(step > leftbuffer){
            step = leftbuffer;
        }
        // <= left ok
    }
    if(step < 0){
        size_t lappedbuffer = lapped();
        if(step < -1 * lappedbuffer){
            step = -1 * lappedbuffer;
        }
        // >= lapped ok
    }
    pos_ += step;

    return step;
}

//////////////////////////////////byte_stream/////////////////////////////////////////////////////
};//!namespace zpkg