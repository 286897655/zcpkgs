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

#include "zio/io_socket.h"
#if defined(Z_SYS_WINDOWS)
#include <WinSock2.h>
#elif defined(Z_SYS_LINUX)
#include <sys/socket.h>
#include <linux/in.h>
#include <unistd.h>
#endif
#include <zlog/log.h>
#include "uv_io_error.h"

namespace zio{

static int sk_family_native(io_socket_t::family _family){
    switch (_family)
    {
        case io_socket_t::family::IPv4:
            return AF_INET;
        case io_socket_t::family::IPv6:
            return AF_INET6;
        case io_socket_t::family::Unix:
            return AF_UNIX;
        default:
            break;
    }
    return AF_UNSPEC;
}

static int sk_type_native(io_socket_t::type _type){
    switch (_type)
    {
    case io_socket_t::type::Stream:
        return SOCK_STREAM;
    case io_socket_t::type::Datagrams:
        return SOCK_DGRAM;
    case io_socket_t::type::Raw:
        return SOCK_RAW;
    default:
        break;
    }
    Z_ASSERT_W(0,"unsupport socket type");
    return 0;
}

static int sk_ip_protocol_native(io_socket_t::type _type){
    switch (_type)
    {
    case io_socket_t::type::Stream:
        return IPPROTO_TCP;
    case io_socket_t::type::Datagrams:
        return IPPROTO_UDP;
    case io_socket_t::type::Raw:
        return IPPROTO_RAW;
    default:
        break;
    }
    return IPPROTO_IP;
}

io_socket_t* io_socket_t::create(io_loop_t* loop,io_fd_t fd){
    return new io_socket_t(loop,fd);
}
io_socket_t* io_socket_t::create(io_loop_t* loop,family _family,type _type){
    io_fd_t sk_fd = ::socket(sk_family_native(_family),sk_type_native(_type),0);
    if(sk_fd == invalid_io_fd_t){
        zlog("zio:create socket error for{}",uv_strerror(uv_last_error()));
        return nullptr;
    }
    return new io_socket_t(loop,sk_fd);
}

io_socket_t::io_socket_t(io_loop_t* loop,family _family,type _type):poller_loop_(loop){
    socket_fd_ = ::socket(sk_family_native(_family),sk_type_native(_type),0);
    if(socket_fd_ == invalid_io_fd_t){

    }
}

io_socket_t::io_socket_t(io_loop_t* loop,io_fd_t fd)
    :socket_fd_(fd),poller_loop_(loop){
    Z_ASSERT(socket_fd_ != invalid_io_fd_t);
    Z_ASSERT(poller_loop_);
}

io_socket_t::~io_socket_t(){
    if(socket_fd_ != invalid_io_fd_t){
        ::close(socket_fd_);
    }
}

bool io_socket_t::bind(const std::string& ip,int port){

}
};//!namespace zio