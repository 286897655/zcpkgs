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

#include "randoms.h"
#include <chrono>
#include <thread>
#include "strings.h"

namespace zpkg { 
Random& Random::TLSInstance(){
    // seed with thread id
    static thread_local Random instance((uint32_t)(std::hash<std::thread::id>()(std::this_thread::get_id())));
    return instance;
}

Random::Random(uint32_t seed):rng_(seed){
    // seed with pamameter
}

Random::Random():rng_(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()){
    // seed with time
}

Random::~Random(){

}

std::string Random::RandomString(size_t length,bool has_upper,bool has_lower,bool has_number,bool has_special){
    // chaset
    std::string char_set;
    if(has_upper){
        char_set += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    if(has_lower){
        char_set += "abcdefghijklmnopqrstuvwxyz";
    }
    if(has_number){
        char_set += "0123456789";
    }
    if(has_special){
        char_set += "!@#$%^&*()_-+=[]{}|;:,.<>?";
    }
    if(char_set.empty()){
        return kstrings::kEmpty;
    }
    
    std::uniform_int_distribution<size_t> dist(0,char_set.size() - 2);
    std::string result;
    result.resize(length);
    for (size_t i = 0; i < length; ++i) {
        result[i] = char_set[dist(rng_)];
    }
    return result;
}

std::string Random::RandomHexString(size_t length){
    static constexpr const char kHexChars[] = "0123456789abcdef";

    std::uniform_int_distribution<> dis(0, 15);
    std::string ret;
    ret.resize(length);
    for (size_t i = 0; i < length; ++i) {    
        ret[i] = kHexChars[dis(rng_)];
    }
    return ret;
}

};//! namespace zpkg