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
 * @brief rtp/rtcp protocol 
 */
#ifndef AVM_PROTO_RTP_RTCP_H_
#define AVM_PROTO_RTP_RTCP_H_ 

#include <string>
#include <cstdint>

namespace avm { 


/*
*  0                   1                   2                   3
*  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* |V=2|P|X|  CC   |M|     PT      |       sequence number         |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* |                           timestamp                           |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* |           synchronization source (SSRC) identifier            |
* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
* |            contributing source (CSRC) identifiers             |
* |                             ....                              |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
class RtpPacket {
public:
    enum{
        RTP_HEADER_SIZE = 12
    };
    /// @brief 
    /// @param bytes 
    /// @param sizeBytes 
    /// @param ssrc 
    /// @return 
    static bool OverrideSSRC(uint8_t* const bytes,size_t sizeBytes,uint32_t ssrc);
    /// @brief 
    /// @param bytes 
    /// @param sizeBytes 
    /// @param out_ssrc 
    /// @return 
    static bool ReadSSRC(const uint8_t* const bytes,size_t sizeBytes,uint32_t* out_ssrc);
    /// @brief 
    /// @param bytes 
    /// @param sizeBytes 
    /// @return 
    static bool Valid(const uint8_t* const bytes,size_t sizeBytes);

    // std::string Dump() const;
};



}//!namespace avm

#endif//!AVM_PROTO_RTP_RTCP_H_