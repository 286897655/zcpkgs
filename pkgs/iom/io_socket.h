#pragma once

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

#ifndef IOM_IO_SOCKET_H_
#define IOM_IO_SOCKET_H_

#include <netinet/in.h>
#include <sys/un.h>


#include <string>

namespace iom {

// socket type on linux
// enum __socket_type
// {
//   SOCK_STREAM = 1,		/* Sequenced, reliable, connection-based
// 				   byte streams.  */
// #define SOCK_STREAM SOCK_STREAM
//   SOCK_DGRAM = 2,		/* Connectionless, unreliable datagrams
// 				   of fixed maximum length.  */
// #define SOCK_DGRAM SOCK_DGRAM
//   SOCK_RAW = 3,			/* Raw protocol interface.  */
// #define SOCK_RAW SOCK_RAW
//   SOCK_RDM = 4,			/* Reliably-delivered messages.  */
// #define SOCK_RDM SOCK_RDM
//   SOCK_SEQPACKET = 5,		/* Sequenced, reliable, connection-based,
// 				   datagrams of fixed maximum length.  */
// #define SOCK_SEQPACKET SOCK_SEQPACKET
//   SOCK_DCCP = 6,		/* Datagram Congestion Control Protocol.  */
// #define SOCK_DCCP SOCK_DCCP
//   SOCK_PACKET = 10,		/* Linux specific way of getting packets
// 				   at the dev level.  For writing rarp and
// 				   other similar things on the user level. */
// #define SOCK_PACKET SOCK_PACKET

//   /* Flags to be ORed into the type parameter of socket and socketpair and
//      used for the flags parameter of paccept.  */

//   SOCK_CLOEXEC = 02000000,	/* Atomically set close-on-exec flag for the
// 				   new descriptor(s).  */
// #define SOCK_CLOEXEC SOCK_CLOEXEC
//   SOCK_NONBLOCK = 00004000	/* Atomically mark descriptor(s) as
// 				   non-blocking.  */
// #define SOCK_NONBLOCK SOCK_NONBLOCK
// };
enum socket_type_t {
    SOCKET_INVALID          = -1,// INVALID
    SOCKET_UNKNOWN          = 0, // UNKNOWN
    SOCKET_STREAM           = SOCK_STREAM, // TCP
    SOCKET_DATAGRAMS        = SOCK_DGRAM, // UDP
    SOCKET_RAW              = SOCK_RAW    // Raw protocol interface
};

enum socket_family_t {
    SOCKET_FAMILY_UNSPEC    = AF_UNSPEC,
    SOCKET_FAMILY_UNIX      = AF_UNIX,
    SOCKET_FAMILY_IPV4      = AF_INET,
    SOCKET_FAMILY_IPV6      = AF_INET6,
};
static_assert(AF_UNSPEC == 0,"AF_UNSPEC != 0");

enum socket_end_t {
    SOCKET_END_LOCAL,
    SOCKET_END_PEER
};

class SocketAddr final{
public:
    // create from socket-sockaddr
    explicit SocketAddr(const struct sockaddr* addr);
    explicit SocketAddr(const struct sockaddr_storage* addr);
    // create from dotted decimal notation ip and port
    explicit SocketAddr(const char* ip,int port);

    template<typename T>
    T* As() const{
        return reinterpret_cast<T*>(sockaddr());
    }

    socket_family_t Family() const;

    struct sockaddr* sockaddr() const;

private:
    union data_storage
    {
        struct sockaddr sa_any;
        struct sockaddr_in sa_v4;
        struct sockaddr_in6 sa_v6;
        struct sockaddr_un sa_unix;
    }storage_;
};



};//!namespace iom

#endif//!IOM_IO_SOCKET_H_