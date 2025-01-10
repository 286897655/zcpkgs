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

#ifndef ZPKG_URL_H_
#define ZPKG_URL_H_

#include <zpkg/error.h>

namespace zpkg{

struct url_ctx{
    std::string schema;
    std::string host;
    int port;
    std::string path;
    std::string base;
    std::string param;
};

/**
 * split url to separate part
 * such as http://127.0.0.1:8080/xxxx/xxx.m3u8
 * rtsp://127.0.0.1:254/xxx/xxx
 * ---> protocol http
 *      hostport 127.0.0.1:8080
 *      path /xxxx/
 *      base xxx.m3u8
 * ---> protocol rtsp
 *      hostport 127.0.0.1:254
 *      path /xxx/
 *      base xxx 
*/
zerror_code_t url_parser(const std::string& url,url_ctx* url_ctx);

};//!namespace zpkg

#endif //!ZPKG_URL_H_