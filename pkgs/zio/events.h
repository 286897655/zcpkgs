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

#ifndef ZIO_EVENTS_H_
#define ZIO_EVENTS_H_

#include <atomic>
#include <functional>
#include <zpkg/times.h>
#include <zpkg/utility.h>
#include <zpkg/execution.h>
#include <zio/utility.h>

/**
 * 包含基于事件循环的三大件
 * 多路复用,定时器,事件循环线程池
*/
namespace zio{

enum event_poll_type{
    // 0000
    EVENT_LT = 0,// default EPOLLLT
    // 0001
    EVENT_READ = 1 << 0,//EPOLLIN = 0x001
    // 0010
    EVENT_WRITE = 1 << 2,//EPOLLOUT = 0x004
    // 0100
    EVENT_ERROR = 1 << 3,//(EPOLLERR | EPOLLHUP)
    // 10000
    EVENT_ET = 1 << 4, // EPOLLET = 1u << 31
};

class event_poll_handler{
public:
    virtual void in_event() = 0;
    virtual void out_event() = 0;
};

using poll_handle_t = void*;
class epoll_poller;
class event_poller_t{
public:
    event_poller_t();
    ~event_poller_t();

    void poll(int timeout = -1);
    poll_handle_t add_fd(io_fd_t fd,int poll_event,event_poll_handler* handler);
    // poll_handle will be delete later,caller do not use poll_handle again
    void rm_fd(poll_handle_t handle);
    void set_in_event(poll_handle_t handle);
    void reset_in_event(poll_handle_t handle);
    void set_out_event(poll_handle_t handle);
    void reset_out_event(poll_handle_t handle);
    uint32_t load();
private:
    epoll_poller* epoll_poller_;
    Z_DISABLE_COPY_MOVE(event_poller_t);
};

using Func = zpkg::Func;

class event_executor : public virtual zpkg::executor{
public:
virtual void async(Func&& func) = 0;
virtual void  sync(Func&& func) = 0;
};

// 多路复用器,一个事件线程一个
class event_poller_t;
// 每一个event_loop 持有一个 event_poller
class event_loop_t;
// 定时器
class event_timer_t;
// 线程池管理
class events_ctx{
public:
    static event_loop_t* default_loop();
    static event_loop_t* this_thread_loop();
    static event_loop_t* create_this_thread_loop();
    static size_t create_loop_pool(size_t count = 0);
};

/**
 * design:
 * one loop per thread event based reacotr
*/
class event_loop_t : public virtual event_executor{
public:
    event_poller_t* poller() const;
    void run_loop() const;
    bool is_this_thread_loop() const;
    uint32_t load() const;
    void set_loop_name(const char* name);

public:
    virtual void add(Func&& func) override;
    virtual void async(Func&& func) override;
    virtual void  sync(Func&& func) override;

private:
    //should use factory method to create,can't not create by caller
    event_loop_t();
    ~event_loop_t();
    class event_loop_impl* loop_impl_;
    friend events_ctx;
    friend event_timer_t;
};


using boolean_call_back = std::function<bool(void)>;
class time_after_task{
public:
    time_after_task(z_time_t after_msec,boolean_call_back&& boolean_cb);
    ~time_after_task() =default;

    z_time_t time_after() const{
        return after_msec_;
    }
    bool call() const;
    void cancel();

private:
    z_time_t after_msec_;
    boolean_call_back boolean_cb_;
    std::atomic_bool canceled_;
};

class event_timer_t{
public:
    explicit event_timer_t(event_loop_t* loop);
    ~event_timer_t();

    // 返回值false表示不再重复触发，返回值true表示重复触发
    void reset(boolean_call_back&& cb,z_time_t msec_interval);
    // 取消定时器
    void cancel();

private:
    Z_DISABLE_COPY_MOVE(event_timer_t);
    event_loop_impl* loop_impl_;
    std::weak_ptr<time_after_task> weak_call_;
};

};//!namespace zio

#endif //!ZIO_EVENTS_H_