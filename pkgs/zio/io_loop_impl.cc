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

#include "io_loop_impl.h"
#include "wake_up_pipe.h"
#include <zlog/log.h>

namespace zio{
io_loop_impl::io_loop_impl(){
    // create poller
    loop_poller_ = new io_poller_t();
    // create thread weak up
    wake_up_ = new wake_up_pipe_t(loop_poller_,this);
}

io_loop_impl::~io_loop_impl(){
    Z_DELETE_P(wake_up_);
    Z_DELETE_P(loop_poller_);
}

int io_loop_impl::run(){
    while(1){
        // get poll timeout time and flush time after task
        int poll_time = execute_time_after_task();
        loop_poller_->poll(poll_time);
    }

    return Z_INT_SUCCESS;
}

// void io_loop_impl::start(){
//     // set thread name
//     if(!loop_thread_name_.empty()){
//         pthread_setname_np(pthread_self(),loop_thread_name_.c_str());
//     }
// }

void io_loop_impl::async(Func&& func){
    // not in this thread insert to tasklist
    {
        std::lock_guard<std::mutex> lock(task_mtx_);
        async_tasks_.emplace_back(func);
    }
    // wake up poller
    wake_up_->wake_up();
}

void io_loop_impl::on_wake_up(){
    // TODO
    //Z_ASSERT(is_this_thread_loop());

    // flush task's
    std::vector<Func> swap_task;
    {
        std::lock_guard<std::mutex> lock(task_mtx_);
        swap_task.swap(async_tasks_);
    }
    try{
        for(const auto& func : swap_task){
            func();
        }
    }catch(const std::exception& ex){
        zlog("zio:do call back got exception:{}",ex.what());
    }catch(...){
        zlog("zio:do call back got unhandled exception");
    }
}

void io_loop_impl::add_time_task(const SharedTimerTask& task){
    z_time_t time_point = zpkg::times::now_ms() + task->time_after();

    async([time_point,task,this](){
        time_after_tasks_.emplace(time_point,task);
    });
}
int io_loop_impl::execute_time_after_task(){
    int timed = 0;
    // no task to do
    if(time_after_tasks_.empty())
        return timed;

    z_time_t now_time = zpkg::times::now_ms();
    decltype(time_after_tasks_)::const_iterator it;
    do{
        it = time_after_tasks_.begin();
        // multimap is sorted on insert we just check first item
        if(it->first > now_time){
            // no task had to do
            // TODO zhaoj fix overflow int????
            timed = it->first - now_time;
            return timed;
        }
        // execute expired task
        // save and remove the task
        {
            SharedTimerTask task = it->second;
            time_after_tasks_.erase(it);
            try{
                if(task->call()){
                    // return true wo should add again
                    z_time_t time_point = now_time + task->time_after();
                    time_after_tasks_.emplace(time_point,task);
                }
            }catch(const std::exception& ex){
                zlog("zio:time after task call with excetion:{}",ex.what());
            }
        }
    }while(!time_after_tasks_.empty());

    return timed;
}


};//!namespace zio