/** 
 * @copyright Copyright © 2020-2025 zhaoj
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

 #include "rtsp.h"
 #include <zpkg/strings.h>

namespace avm{
    
//rtsp://[用户名:密码@]主机[:端口]/路径
std::string rtsp_url_add_usr_pwd(const std::string& url,const std::string& usr,const std::string& pwd){
    size_t found = url.find("rtsp://");
    if(found != 0){
        // not start or not found
        return url;
    }
    std::string no_protocol = url.substr(0,sizeof("rtsp://") - 1);
    // check whether rtsp url has usr and pwd
    size_t found_comma = no_protocol.find(':');
    size_t found_at = no_protocol.find('@');
    // no comma or no @ means no usr and pwd
    // with comma and with @ and comma less @ means has usr and pwd
    if(found_comma == std::string::npos || found_at == std::string::npos ){
        // no usr and pwd
        return "rtsp://" + usr + ":" + pwd + "@" + no_protocol;
    }

    // founded
    if(found_comma < found_at){// : must befor @
        return url;
    }

    return "rtsp://" + usr + ":" + pwd + "@" + no_protocol;
}

 };//!namespace avm
