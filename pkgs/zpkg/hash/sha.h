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
 * @brief sha1 and sha2 take from ffmpeg
 */

#ifndef ZPKG_HASH_SHA_H_
#define ZPKG_HASH_SHA_H_

#include <zpkg/hash/hash.h>
#include <cstdint>

namespace zpkg{

enum sha_algorithm {
    SHA1 = hash::algorithm::SHA1,
    SHA256 = hash::algorithm::SHA256
};

class sha final{
public:
    explicit sha(sha_algorithm algorithm);
    ~sha();

    void update(const uint8_t* bytes,size_t byteSize);
    void update(const std::string& str);

    void reset(sha_algorithm algorithm);

    // result of hex string
    std::string hash_hex();
    // result of binary buffer with \0 end
    std::string hash_bin();
private:
    struct SHA_CTX* sha_ctx_;
};
} //! namespace zpkg

#endif //!ZPKG_HASH_SHA_H_