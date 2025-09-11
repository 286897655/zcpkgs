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

#ifndef ZIO_IO_SOCKET_H_
#define ZIO_IO_SOCKET_H_

#include <zio/io_ctx.h>

namespace zio{

class i_address{
public:
    virtual ~i_address();
    virtual std::string get_local_ip() = 0;
    virtual int get_local_port() = 0;
    virtual std::string get_peer_ip() = 0;
    virtual int get_peer_port() = 0;
};

class ip_addr_v4{

};

class ip_addr_v6{

};

class ip_addr{
private:
    typedef union{
        ip_addr_v4 v4_addr_;
        ip_addr_v6 v6_addr_;
    }ip_addr_v46;
    ip_addr_v46 v46_addr_;

};


// 该对象创建释放都很频繁而且容易异步操作，因此使用智能指针管理
class io_socket_t : public i_address,public poll_event_handler,public zpkg::apply_shared<io_socket_t>{
private:
    io_fd_t socket_fd_;
    io_loop_t* poller_loop_;
public:
    enum class family{
        IPv4 ,// AF_INET
        IPv6 ,// AF_INET6
        Unix // AF_UNIX
    };
    enum class type{
        Invalid          = -1,// INVALID
        Unknown          = 0, // UNKNOWN
        Stream           = 1, // TCP
        Datagrams        = 2, // UDP
        Raw              = 3
    };
public:
    static io_socket_t* create(io_loop_t* loop,io_fd_t fd);
    static io_socket_t* create(io_loop_t* loop,family _family,type _type);

    explicit io_socket_t(io_loop_t* loop,family _family,type _type);
    virtual ~io_socket_t();

    bool bind(const std::string& ip,int port);
    bool listen();
    bool connect(const std::string& ip,int port);



    // i_address override
    virtual std::string get_local_ip() override;
    virtual int get_local_port() override;
    virtual std::string get_peer_ip() override;
    virtual int get_peer_port() override;

    // poll_event_handler
    virtual void in_event() override;
    virtual void out_event() override;
private:
    io_socket_t(io_loop_t* loop,io_fd_t fd);
    Z_DISABLE_COPY_MOVE(io_socket_t);
};

/**
 * represent a conntion
*/
class io_conn_t : public zpkg::apply_shared<io_conn_t>{
public:
    static io_conn_t::shared create(io_loop_t* loop,io_fd_t fd);
public:
    
private:
    Z_DISABLE_COPY_MOVE(io_conn_t);
    io_poller_t* poller_;
};

};//!namespace zio

#endif//!ZIO_NIO_SOCKET_H_