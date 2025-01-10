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

#include "wake_up_pipe.h"
#include <unistd.h>
#include <zlog/log.h>
#include "uv_io_error.h"
#include "io_utility.h"
namespace zio{

wake_up_pipe_t::wake_up_pipe_t(io_loop_t* loop)
    :loop_(loop),poll_handle_(nullptr)
{
    pipe_fd_[0] = -1;
    pipe_fd_[1] = -1;
    re_open();
}

wake_up_pipe_t::~wake_up_pipe_t()
{
    close();
}

void wake_up_pipe_t::wake_up()
{
    int ret;
    do{
        ret = ::write(pipe_fd_[1],"",1);
    }while(-1 == ret && uv_last_error() == UV_EINTR);
}

void wake_up_pipe_t::in_event(){
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
        // read eof or non-EAGAIN error,pipe is invalid, reopen it
        zlog_warn("zio:wake up pipe invalid,reopen it");
        // delete event poll from poller
        loop_->poller()->rm_fd(poll_handle_);
        re_open();
        // add event poll to poller
        poll_handle_ = loop_->poller()->add_fd(pipe_fd_[0],event_read,this);
    }

    // wake for loop
    loop_->on_wake_up();
}

void wake_up_pipe_t::out_event(){
    Z_ASSERT("zio:wake_up_pipe has no out event!" == 0);
}

void wake_up_pipe_t::re_open(){
    if(::pipe(pipe_fd_) < 0){
        throw std::runtime_error("create posix pipe fail,throw for aborted");
    }
    fd_control::make_non_blocking(pipe_fd_[0]);
    fd_control::make_close_on_exec(pipe_fd_[0]);
    fd_control::make_non_blocking(pipe_fd_[1]);
    fd_control::make_close_on_exec(pipe_fd_[1]);

    // add to poller
    poll_handle_ = loop_->poller()->add_fd(pipe_fd_[0],event_read,this);
}

void wake_up_pipe_t::close(){
    if(pipe_fd_[0] != invalid_io_fd_t){
        ::close(pipe_fd_[0]);
        pipe_fd_[0] = invalid_io_fd_t;
    }
    if(pipe_fd_[1] != invalid_io_fd_t){
        ::close(pipe_fd_[1]);
        pipe_fd_[1] = invalid_io_fd_t;
    }
}
};//!namespace zio