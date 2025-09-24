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

#ifndef ZIO_EVENT_LOOP_IMPL_H_
#define ZIO_EVENT_LOOP_IMPL_H_

#include <map>
#include <mutex>
#include "events.h"

namespace zio{
using shared_timer_task = std::shared_ptr<time_after_task>;
class wake_up_pipe_t;

class event_loop_impl{
public:
    event_loop_impl();
    ~event_loop_impl();

    event_poller_t* poller() const;
    void run();
    void async(Func&& func);
    // run in loop thread to wake up thread
    void on_wake_up();
    void add_time_task(const shared_timer_task& task);
    int execute_time_after_task();

private:
    event_poller_t* poller_ = nullptr;
    wake_up_pipe_t* wake_up_ = nullptr;
    std::mutex task_mtx_;
    std::vector<Func> async_tasks_;
    std::multimap<z_time_t, shared_timer_task> time_after_tasks_;
};

};//!namespace zio

#endif//!ZIO_EVENT_LOOP_IMPL_H_