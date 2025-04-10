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

#ifndef ZIO_IO_BUF_H_
#define ZIO_IO_BUF_H_

#include <string>

namespace zio{

/**
 * io_buf manages heap-allocated byte buffers.
 * io_buf objects are intended to be used primarily for networking code
 * io_buf_t manages many io_block_t
 * Data Layout
 * -----------
 *
 * io_block_t objects contains a pointer to the buffer and information about which
 * segment of the buffer contains valid data.
 *
 *      +------------+
 *      | io_block_t |
 *      +------------+
 *       /
 *      |            |----- length() -----|
 *      v
 *      +------------+--------------------+-----------+
 *      | headroom   |        data        |  tailroom |
 *      +------------+--------------------+-----------+
 *      ^            ^                    ^           ^
 *      head()   data()               end()      tail()
 *
 *      |------------ capacity(default use 4k) -------|
 * 
 */
class io_block_t{
public:
    explicit io_block_t(size_t capacity = 4096);

public:
    uint8_t* head() const;
    uint8_t* data() const;
    uint8_t* end() const;
    uint8_t* tail() const;

    size_t length() const;
    size_t capacity() const;

private:
    size_t length_;
    uint8_t* data_;

    size_t capacity_;
    uint8_t* block_;
};

class io_buf_t{
private:
    io_block_t* block_;
    io_block_t* next_;
    io_block_t* prev_;

};

class io_buffer{

};



};//!namespace zio


#endif//!ZIO_IO_BUF_H_