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
 * @brief socket tools
 */

 #ifndef ZPKG_SOCKETS_H_
 #define ZPKG_SOCKETS_H_

#include <zpkg/utility.h>
#include <zpkg/errors.h>
#if defined(Z_SYS_WINDOWS)
    #include <WinSock2.h>
#elif defined(Z_SYS_LINUX)
    #include <sys/socket.h>
#endif

namespace zpkg { 

enum socket_type_t{
    SOCKET_INVALID = -1,
    SOCKET_STREAM = 0,
    SOCKET_DATAGRAM = 1,
    SOCKET_RAW = 2,
    SOCKET_UNKNOWN = 3
};

enum socket_end_t{
    SOCKET_END_LOCAL = 0,
    SOCKET_END_PEER = 1
};


class ip_address;
class sockets{
public:
    static bool ip_valid(const char* ip);
    static bool ipv4_valid(const char* ip);
    static bool ipv6_valid(const char* ip);

    //static Status_t 
};

class ip_address{
public:
    explicit ip_address(const std::string& addr_str);

    bool valid() const;
};

}//!namesapce zpkg

 #endif //!ZPKG_SOCKETS_H_