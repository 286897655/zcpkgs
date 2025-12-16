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
#include "rtp_rtcp.h"
#include <zpkg/memory.h>
#include <zcpkgs_common.h>

namespace avm { 
///////////////////////////RtpPacket///////////////////////////////////
bool RtpPacket::OverrideSSRC(uint8_t* const bytes,size_t sizeBytes,uint32_t ssrc){
    if(sizeBytes < RTP_HEADER_SIZE){
        return false;
    }

    Z_WBE32(bytes + 8,ssrc);
    return true;
}

bool RtpPacket::ReadSSRC(const uint8_t* const bytes,size_t sizeBytes,uint32_t* out_ssrc){
    if(sizeBytes < RTP_HEADER_SIZE){
            return false;
    }
    
    Z_ASSERT(out_ssrc);Z_ASSERT(bytes);

    *out_ssrc = Z_RBE32(bytes + 8);
    return true;
}

bool RtpPacket::Valid(const uint8_t* const bytes,size_t sizeBytes){
    if(sizeBytes < RTP_HEADER_SIZE){
        return false;
    }
    Z_ASSERT(bytes);
    // check payload type
    uint8_t pt = (*(bytes + 1)) & 0x7F;
    return (pt < 64 || pt >= 96);
}
///////////////////////////RtpPacket///////////////////////////////////
};//! namespace avm