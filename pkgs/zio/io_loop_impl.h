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
#ifndef ZIO_IO_LOOP_IMPL_H_
#define ZIO_IO_LOOP_IMPL_H_

#include <map>
#include <mutex>
#include <vector>
#include <zpkg/utility.h>
#include <zpkg/times.h>
#include "zio/io_loop_ctx.h"

namespace zio{

using Func = std::function<void()>;
using SharedTimerTask = std::shared_ptr<io_timer_t::time_after_task>;
class wake_up_pipe_t;
class io_loop_impl{
public:
    explicit io_loop_impl(const std::string& threadname);
    ~io_loop_impl();

    void run();
    void start();
    void move_to_thread(std::thread* thread){
        run_thread_ = thread;
    }
    io_poller_t* poller() const{
        return loop_poller_;
    }
    void async(Func&& func);
    // run in loop thread
    void on_wake_up();
    void add_time_task(const SharedTimerTask& task);
    int execute_time_after_task();
private:
    std::string loop_thread_name_;
    std::thread* run_thread_ = nullptr;
    io_poller_t* loop_poller_ = nullptr;
    wake_up_pipe_t* wake_up_ = nullptr;
    std::mutex task_mtx_;
    std::vector<Func> async_tasks_;
    std::multimap<z_time_t, SharedTimerTask> time_after_tasks_;
};

};//!namespace zio

#endif//ZIO_IO_LOOP_IMPL_H_