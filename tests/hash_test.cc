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
#include <zlog/log.h>
#include <zpkg/hash/hash.h>
#include <zpkg/hash/sha1.h>
#include <zpkg/base64.h>

int main(int argc,char** argv){
    zlog::logger::create_defaultLogger();

    std::string origin_str = "hello world";
    std::string sha_160_str = zpkg::hash::sha::hash(zpkg::hash::SHA1,origin_str);
    std::string sha1_str = zpkg::SHA1::encode(origin_str);
    zlog("sha 160:{}",sha_160_str);
    zlog("sha1:{}",sha1_str);
    Z_ASSERT(sha_160_str == sha1_str);

    std::string origin1_str = "Permission is hereby granted, free of charge, to any person obtaining a copy\n";
    std::string origin2_str = "of this software and associated documentation files (the Software), to deal\n";
    std::string origin3_str = "in the Software without restriction, including without limitation the rights\n";
    origin_str = origin1_str+origin2_str+origin3_str;
    sha_160_str = zpkg::hash::sha::hash(zpkg::hash::SHA1,origin_str);
    sha1_str = zpkg::SHA1::encode(origin_str);
    zlog("sha 160:{}",sha_160_str);
    zlog("sha1:{}",sha1_str);
    Z_ASSERT(sha_160_str == sha1_str);

    // stage
    zpkg::hash::sha hashtool(zpkg::hash::SHA1);
    hashtool.update(origin1_str);
    hashtool.update(origin2_str);
    hashtool.update(origin3_str);

    std::string stage_sha_160_str = hashtool.hash();

    zpkg::SHA1 sha1;
    sha1.update(origin1_str);
    sha1.update(origin2_str);
    sha1.update(origin3_str);
    std::string stage_sha1_160_str = sha1.final();

    zlog("sha 160:{}",stage_sha_160_str);
    zlog("sha1:{}",stage_sha1_160_str);
    Z_ASSERT(stage_sha_160_str == stage_sha1_160_str);

    // test onvif digest
    std::string nonce = "rwcnybNnfOzOiEJNEddqEA==";
    std::string created = "2025-10-11T09:18:05Z";
    std::string passwd = "L6@jp4Yx4!";

    std::string onvif_device_tool_digest = "hwMa9EF+GmRbdGUmWl3INiK5t0k=";
    std::string onvif_digest = zpkg::base64_encode(zpkg::hash::sha::hash_bin(zpkg::hash::SHA1,(zpkg::base64_decode(nonce)+created+passwd)));
    zlog("onvif_digest:{}",onvif_digest);
    zlog("onvif_device_tool_digest:{}",onvif_device_tool_digest);
}