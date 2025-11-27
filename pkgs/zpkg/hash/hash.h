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

#ifndef ZPKG_HASH_H_
#define ZPKG_HASH_H_

#include <string>

namespace zpkg{ 

class hash final{
public:
    /**
     * @brief define hash algorithms
     * md5
     * sha1
     * sha256
     * 
     */
    enum algorithm{
        MD5,
        /**
         * SHA-1 and SHA-256 (Secure Hash Algorithm) hash function implementations.
         * - SHA-1: 160 bits
         * - SHA-256: 256 bits, as a variant of SHA-2
        */
        SHA1 = 160,
        SHA256 = 256
    };

    // compute hash of a string, excluding final zero with hex string
    static std::string hash_hex(algorithm algorithm,const std::string& str);
    // compute hash of a memory block return with hex string
    static std::string hash_hex(algorithm algorithm,const uint8_t* bytes,size_t byteSize);
    // compute hash of a string, excluding final zero with binary buffer
    static std::string hash_bin(algorithm algorithm,const std::string& str);
    // compute hash of a memory block return binary buffer with \0 end
    static std::string hash_bin(algorithm algorithm,const uint8_t* bytes,size_t byteSize);
};

};//!namesapce zpkg
#endif//!ZPKG_HASH_H_