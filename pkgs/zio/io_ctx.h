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

#ifndef ZIO_IO_LOOP_H_
#define ZIO_IO_LOOP_H_

#include <string>
#include <thread>
#include <atomic>
#include <zpkg/utility.h>
#include <zpkg/times.h>
#include <zpkg/execution.h>

namespace zio{

/**
 * design:
 * one loop per thread 
*/

// impl for io loop
class io_loop_impl;
class io_poller_t;

using Func = zpkg::Func;

class io_executor : public virtual zpkg::executor{
public:
    virtual ~io_executor()=default;

    virtual void async(Func&& func) = 0;
    virtual void  sync(Func&& func) = 0;
};

class io_loop_t : public virtual io_executor{
public:
    static io_loop_t* default_loop();
    // next io loop in pool use round-robin
    //static io_loop_t* pool_next_loop();
    // get io loop in pool with min io_fd_t
    //static io_loop_t* pool_min_loop();
    // get io loop in pool with lower load 
    //static io_loop_t* pool_low_load_loop();
    static io_loop_t* create_this_thread_loop();
    static io_loop_t* this_thread_loop();
    static size_t create_loop_pool(size_t count = 0);
public:
    int run_loop();
    bool is_this_thread_loop() const;
    io_poller_t* poller() const;
    uint32_t load();
    void set_loop_name(const std::string name);
public:
    virtual void async(Func&& func) override;
    virtual void sync(Func&& func) override;
    virtual void add(Func&& func) override;
private:
    io_loop_t();
    virtual ~io_loop_t();
    Z_DISABLE_COPY_MOVE(io_loop_t)

private:
    io_loop_impl* loop_impl_;
    friend class io_timer_t;
};

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

    void poll(int timeout = -1);
    poll_handle_t add_fd(io_fd_t fd,int poll_event,poll_event_handler* handler);
    // poll_handle will be delete later,caller do not use poll_handle again
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

// 先用std::multi_map实现 后面考虑更改为内核提供的timerfd
// 自己实现的这个timer时间判断获取的时候需要更改为使用ns时间 因为获取ns时间是用monotici clock获取的
class io_timer_impl;
// thread safe timer,should be create on specific loop
class io_timer_t{
public:
    class time_after_task{
    public:
        time_after_task(z_time_t after_msec,std::function<bool(void)>&& boolean_cb);
        ~time_after_task() =default;

        z_time_t time_after() const{
            return after_msec_;
        }
        bool call() const;
        void cancel();

    private:
        z_time_t after_msec_;
        std::function<bool(void)> boolean_cb_;
        std::atomic_bool canceled_;
    };
public:
    explicit io_timer_t(io_loop_t* loop);
    ~io_timer_t();

    using boolean_call_back = std::function<bool(void)>;
    // 返回值false表示不再重复触发，返回值true表示重复触发
    void reset(boolean_call_back&& cb,z_time_t msec_interval);
    // 取消定时器
    void cancel();
private:
    Z_DISABLE_COPY_MOVE(io_timer_t)
    std::weak_ptr<time_after_task> weak_call_;
    io_loop_impl* loop_impl_;
};

};//!namespace

#endif//!ZIO_IO_LOOP_H_