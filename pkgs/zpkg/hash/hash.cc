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

#include "hash.h"
#include "md5.h"
#include "sha.h"

using MD5Class = zpkg::MD5;
namespace zpkg{
std::string hash::hash_hex(algorithm algo,const std::string& str){
    switch (algo)
    {
        case algorithm::MD5:
            {
                MD5Class md5 = MD5Class(str);
                return md5.hexdigest();
            }
            break;
        case algorithm::SHA1:
            {
                sha sha1(sha_algorithm::SHA1);
                sha1.update(reinterpret_cast<const uint8_t*>(str.c_str()),str.size());
                return sha1.hash_hex();
            }
            break;
        case algorithm::SHA256:
            {
                sha sha256(sha_algorithm::SHA256);
                sha256.update(reinterpret_cast<const uint8_t*>(str.c_str()),str.size());
                return sha256.hash_hex();
            }
            break;
        default:
            break;
    }
    return "";
}
std::string hash::hash_hex(algorithm algo,const uint8_t* bytes,size_t byteSize){
    switch (algo)
    {
        case algorithm::MD5:
            {
                MD5Class md5;
                md5.update(bytes,byteSize);
                md5.finalize();
                return md5.hexdigest();
            }
            break;
        case algorithm::SHA1:
            {
                sha sha1(sha_algorithm::SHA1);
                sha1.update(bytes,byteSize);
                return sha1.hash_hex();
            }
            break;
        case algorithm::SHA256:
            {
                sha sha256(sha_algorithm::SHA256);
                sha256.update(bytes,byteSize);
                return sha256.hash_hex();
            }
            break;
        default:
            break;
    }
    return "";
}
std::string hash::hash_bin(algorithm algo,const std::string& str){
    switch (algo)
    {
        case algorithm::MD5:
            {
                MD5Class md5 = MD5Class(str);
                return md5.rawdigest();
            }
            break;
        case algorithm::SHA1:
            {
                sha sha1(sha_algorithm::SHA1);
                sha1.update(reinterpret_cast<const uint8_t*>(str.c_str()),str.size());
                return sha1.hash_bin();
            }
            break;
        case algorithm::SHA256:
            {
                sha sha256(sha_algorithm::SHA256);
                sha256.update(reinterpret_cast<const uint8_t*>(str.c_str()),str.size());
                return sha256.hash_bin();
            }
            break;
        default:
            break;
    }
    return "";
}
std::string hash::hash_bin(algorithm algo,const uint8_t* bytes,size_t byteSize){
    switch (algo)
    {
        case algorithm::MD5:
            {
                MD5Class md5;
                md5.update(bytes,byteSize);
                md5.finalize();
                return md5.rawdigest();
            }
            break;
        case algorithm::SHA1:
            {
                sha sha1(sha_algorithm::SHA1);
                sha1.update(bytes,byteSize);
                return sha1.hash_bin();
            }
            break;
        case algorithm::SHA256:
            {
                sha sha256(sha_algorithm::SHA256);
                sha256.update(bytes,byteSize);
                return sha256.hash_bin();
            }
            break;
        default:
            break;
    }
    return "";
}

}//!namesapce zpkg