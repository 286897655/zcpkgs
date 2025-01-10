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
#include "uv_io_error.h"
namespace zio{

wake_up_pipe_t::wake_up_pipe_t(io_poller_t* poller):poller_(poller),poll_handle_(nullptr){
    pipe_fd_[0] = -1;
    pipe_fd_[1] = -1;
    re_open();
}

wake_up_pipe_t::~wake_up_pipe_t(){
    close();
}

void wake_up_pipe_t::wake_up(){

}

void wake_up_pipe_t::re_open(){
     if(pipe(pipe_fd_) < 0){
            throw std::runtime_error("create posix pipe fail,throw for aborted");
        }
        fd_control::make_non_blocking(pipe_fd_[0]);
        fd_control::make_close_on_exec(pipe_fd_[0]);
        fd_control::make_non_blocking(pipe_fd_[1]);
        fd_control::make_close_on_exec(pipe_fd_[1]);

        // 加入poller监听
        poll_handle_ = poller_->add_fd(pipe_fd_[0],event_read,this);
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