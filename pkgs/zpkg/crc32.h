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

#ifndef ZPKG_CRC32_H_
#define ZPKG_CRC32_H_

#include <cstddef>
#include <cstdint>
#include <string>

namespace zpkg{

// 标准crc32 
// 使用多项式公式0x04C11DB7 0x04C11DB7->翻转为 0xEDB88320
// crc初始值0xFFFFFFFF
// crc结果异或值 0xFFFFFFFF
// 使用输入翻转(Refin) 输出翻转(Refout)
uint32_t crc32_ieee(const uint8_t* data,size_t size);

// 标准crc32 
// 使用多项式公式0x04C11DB7
// crc初始值0xFFFFFFFF
// crc结果异或值 0x00000000
// 不使用输入翻转(Refin) 输出翻转(Refout)
uint32_t crc32_mpeg2(const uint8_t* data,size_t size);

inline uint32_t crc32(const uint8_t* data,size_t size){
    return crc32_ieee(data,size);
}

// CRC32C 最大优势是硬件加速支持，现代 CPU（如 Intel 的 SSE4.2 指令集、ARM 的 Crypto Extensions）提供专门指令，可大幅提升计算速度
// CRC32：传统压缩格式（ZIP、GZIP）、PNG 图像、以太网帧校验等
// CRC32C：存储系统（如 ZFS、EXT4）、网络协议（如 iSCSI）、数据库（如 MySQL）、虚拟机技术等需要高性能校验的场景
// CRC32C：采用 0x1EDC6F41 多项式（也称为 Castagnoli 多项式）
uint32_t crc32c(const uint8_t* data,size_t size);

}

#endif//!ZPKG_CRC32_H_