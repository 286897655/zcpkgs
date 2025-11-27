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
 * @brief strings utility
 * 
 * using like strings::xxxxxx
 */

#include <zpkg/strings.h>
#include <string.h>
#include <iomanip>
#include <random>

namespace zpkg{

namespace strings{

// TODO zhaoj random string 和 random-hex放到randoms中去
std::string random_string(size_t length,bool has_upper,bool has_lower,bool has_number,bool has_special){
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
    std::default_random_engine rng;
    std::uniform_int_distribution<size_t> dist(0,char_set.size() - 1);

    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        result += char_set[dist(rng)];
    }
    return result;
}

std::string random_hex(size_t length){
    static constexpr const char kHexChars[] = "0123456789abcdef";

    std::mt19937 rng;
    std::uniform_int_distribution<> dis(0, sizeof(kHexChars) - 1);
    std::string ret;
    ret.reserve(length);
    for (size_t i = 0; i < length; ++i) {    
        ret += kHexChars[dis(rng)];
    }
    return ret;
}
std::vector<std::string> split(const std::string& origin,const char* delim)
{
    std::vector<std::string> ret;
    size_t last = 0;
    auto index = origin.find(delim, last);
    while (index != std::string::npos) {
        if (index - last > 0) {
            ret.push_back(origin.substr(last, index - last));
        }
        last = index + ::strlen(delim);
        index = origin.find(delim, last);
    }
    if (!origin.size() || origin.size() - last > 0) {
        ret.push_back(origin.substr(last));
    }
    return ret;
}

std::string replace_all(const std::string& origin,const std::string& old_pattern,const std::string& new_pattern)
{
    if(!old_pattern.compare(new_pattern))
        return origin;
    
    size_t pos = 0;
    std::string ret_val = origin;
    while((pos = ret_val.find(old_pattern,pos)) != std::string::npos){
        ret_val.replace(pos,old_pattern.length(),new_pattern);
    }

    return ret_val;
}

std::string extract_field(const char* str,const char* start,const char* end,size_t length){
    if(length == 0){
        length = ::strlen(str);
    }
    if(!str || length == 0){
        // null string or empty string return empty
        return kstrings::kEmpty;
    }
    const char* message_start = str;
    const char* message_end = str + length;
    size_t len = 0;
    if(start){
        // none null start find
        len = ::strlen(start);
        message_start = ::strstr(message_start,start);
        if(!message_start){
            // start not found
            return kstrings::kEmpty;
        }
    }
    message_start += len;
    if(end){
        message_end = ::strstr(message_start,end);
        if(!message_end){
            return kstrings::kEmpty;
        }
    }
    // found start end
    return std::string(message_start,message_end);
}

}//!namespace strings

std::string fmt_hexdump(const uint8_t* data,size_t size,size_t max_size)
{
    size = std::min(size,max_size);

    static constexpr size_t PRINT_NUM = 16;

    std::ostringstream oss;
    {
        // title
        oss << "OFFSET_H" << " ";
        for(size_t i = 0;i < PRINT_NUM;i++){
            oss << std::hex << std::setw(2) << std::setiosflags(std::ios::uppercase) << i << " ";
        }
        oss << "|" << " " << "ASCII" << std::endl;
    }

    {
        // data
        for(size_t i = 0;i < size;i += PRINT_NUM){
            // head of pos
            oss << std::hex << std::setfill('0') << std::setw(8) << std::setiosflags(std::ios::uppercase) << i << " ";
            // print data
            {
                for(size_t j = 0;j< PRINT_NUM;j++){
                    if(i + j < size){
                        oss << std::hex << std::setfill('0') << std::setw(2) << std::setiosflags(std::ios::uppercase) << static_cast<int>(data[i + j]);
                    }else{
                        oss << "  ";
                    }
                    oss << " ";
                }
            }                
            // print ascii
            oss << "|" << " ";
            for(size_t j = 0;j < PRINT_NUM;j++){
                if(i + j < size){
                    if(std::isprint(data[i+j])){
                        oss << static_cast<char>(data[i+j]);
                    }else{
                        oss << ".";
                    }
                }
            }
            oss << std::endl;
        }
    }

    return oss.str();
}

}//!namespace zpkg