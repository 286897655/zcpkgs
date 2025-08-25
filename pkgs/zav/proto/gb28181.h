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

#ifndef ZAV_PROTO_GB28181_H_
#define ZAV_PROTO_GB28181_H_

#include <string>

namespace zav{

/// @brief rtp transport set of gb28181
enum class GB28181_RTP_TRANSPORT : uint8_t {
    INVALID                     = 0x00,
    RTP_SERVER_UDP              = 0x01,// rtp server side with udp
    RTP_SERVER_TCP_SERVER       = 0x02,// rtp server side with tcp server
    RTP_SERVER_TCP_CLIENT       = 0x03,// rtp server side with tcp client

    INVALID_SERVER              = 0x0F,

    RTP_CLIENT_UDP              = 0xF1,// rtp client side with udp
    RTP_CLIENT_TCP_CLIENT       = 0xF2,// rtp client side with tcp client
    RTP_CLIENT_TCP_SERVER       = 0xF3,// rtp client side with tcp server

    INVALID_CLIENT              = 0xFF
};

/// @brief stream type of gb28181 media stream
enum GB28181_STREAM_TYPE{

    GB28181_SSRC_UNIQUE = 0x00,// 以SSRC定流，对流数据做SSRC校验，有SSRC黑名单机制
    GB28181_PORT_UNIQUE = 0x01// 以端口定流，对流数据重打SSRC，有端口黑名单机制(
                       // UDP端口断掉无法决定对方不继续发数据存在不处理ICMP错误，TCP端口关闭也无法决定客户端不继续重连)
};

/// @brief PORT_UNIQUE State of Port (white gray black) for use
enum GB28181_PORT_UNIQUE_STATE{
    // Gray state > 10 second 
    // and get no unknown udp and tcp connection got,can be used serve gb28181 channnel
    GB28181_WHITE_PORT_STATE                   = 0x00,
    // Gray state,listend and no unknown udp and tcp connection got
    GB28181_GRAY_PORT_STATE                    = 0x01,
    // Gray state got unkown udp and tcp connection,black state close listen,it will be in next 10 second
    GB28181_BLACK_PORT_STATE                   = 0x02
};

};//!namespace zav

#endif//!ZAV_PROTO_GB28181_H_