/** 
 * @copyright Copyright © 2020-2024 code by zhaoj
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

#ifndef ZIO_IO_POLLER_H_
#define ZIO_IO_POLLER_H_

#include <string>
#include <functional>
#include <thread>
#include <zpkg/utility.h>
#include <zio/timer.h>
#include <vector>
#include <mutex>

namespace zio{
#if defined _WIN32
// Windows uses a pointer-sized unsigned integer to store the socket fd.
#if defined _WIN64
typedef unsigned __int64 io_fd_t;
#else
typedef unsigned int io_fd_t;
#endif
#else
typedef int io_fd_t;
#endif

enum{
    invalid_io_fd_t = -1//
};

class io_poller_t;
class wake_up_pipe_t;
class io_loop_t{
public:
    using call_back_func = std::function<void()>;
    // 在主线程中先调用一次
    static io_loop_t* default_loop();
    static io_loop_t* next_loop();
    static io_loop_t* min_load_loop();
    static io_loop_t* this_thread_loop();
    static size_t create_loop_pool(const std::string& name="",size_t count = 0);
    
public:
    void async(call_back_func&& callback);
    void run();
    bool is_this_thread_loop() const;
    io_poller_t* poller() const;
    uint32_t load();

private:
    explicit io_loop_t(const std::string& loop_name);
    ~io_loop_t();

    // run in loop thread
    void init();
    void on_wake_up();
private:
    friend class wake_up_pipe_t;
    std::string loop_name_;
    std::thread* loop_thread_;
    io_poller_t* loop_poller_;
    wake_up_pipe_t* wake_up_;
    std::mutex task_mtx_;
    std::vector<call_back_func> async_tasks_;
    //std::multimap<uint64_t, timer_info_t> timers_t;
    Z_DISABLE_COPY_MOVE(io_loop_t)
};

enum poll_event_type{
    // 0000
    event_LT = 0,// default EPOLLLT
    // 0001
    event_read = 1 << 0,//EPOLLIN = 0x001
    // 0010
    event_write = 1 << 2,//EPOLLOUT = 0x004
    // 0100
    event_error = 1 << 3,//(EPOLLERR | EPOLLHUP)
    // 10000
    event_ET = 1 << 4, // EPOLLET = 1u << 31
};
class poll_event_handler{
public:
    virtual void in_event() = 0;
    virtual void out_event() = 0;
};

using poll_handle_t = void*;
class epoll_poller;
class io_poller_t{
public:
    io_poller_t();
    ~io_poller_t();

    void poll();
    poll_handle_t add_fd(io_fd_t fd,int poll_event,poll_event_handler* handler);
    void rm_fd(poll_handle_t handle);
    void set_in_event(poll_handle_t handle);
    void reset_in_event(poll_handle_t handle);
    void set_out_event(poll_handle_t handle);
    void reset_out_event(poll_handle_t handle);
    uint32_t load();

private:
    epoll_poller* epoll_poller_;
    Z_DISABLE_COPY_MOVE(io_poller_t)
};

}//!namespace zio

#endif//!ZIO_IO_POLLER_H_