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

#ifndef ZIO_WAKE_UP_PIPE_H_
#define ZIO_WAKE_UP_PIPE_H_

#include "zio/io_poller.h"

namespace zio{

class wake_up_pipe_t final: public poll_event_handler{
public:
    explicit wake_up_pipe_t(io_poller_t* poller);
    ~wake_up_pipe_t();

    void wake_up();
protected:
    virtual void in_event() override{
        int ret;
        char buf[256];
        do{
            ret = ::read(pipe_fd_[0],buf,sizeof(buf));
            if(ret > 0){
                // pipe data read until eof or empty
                continue;
            }
        }while(-1 == ret && UV_EINTR == uv_last_error());

        if(ret == 0 || uv_last_error() != UV_EAGAIN){
            // 
        }
    }

    virtual void out_event() override{

    }

    virtual void error_event() override{

    }
private:
    void re_open();
    void close();
private:
    io_poller_t* poller_;
    poll_handle_t poll_handle_;
    // 0 for read ,1 for write
    io_fd_t pipe_fd_[2];
};


};//!namespace zio

#endif//!ZIO_WAKE_UP_PIPE_H_