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

#include "event_loop_impl.h"
#include <zlog/log.h>
#include "epoll_poller.h"
#include "wake_up.h"

namespace zio{
//////////////////////////////event_poller_t///////////////////////////////
event_poller_t::event_poller_t():epoll_poller_(epoll_poller::create()){

}
event_poller_t::~event_poller_t(){
    delete epoll_poller_;
    epoll_poller_ = nullptr;
}

void event_poller_t::poll(int timeout){
    epoll_poller_->poll(timeout);
}

poll_handle_t event_poller_t::add_fd(io_fd_t fd,int poll_event,event_poll_handler* handler){
    return epoll_poller_->add_fd(fd,poll_event,handler);
}

// poll_handle will be delete later,caller do not use poll_handle again
void event_poller_t::rm_fd(poll_handle_t handle){
    return epoll_poller_->rm_fd(handle);
}

void event_poller_t::set_in_event(poll_handle_t handle){
    return epoll_poller_->set_in_event(handle);
}

void event_poller_t::reset_in_event(poll_handle_t handle){
    return epoll_poller_->reset_in_event(handle);
}

void event_poller_t::set_out_event(poll_handle_t handle){
    return epoll_poller_->set_out_event(handle);
}

void event_poller_t::reset_out_event(poll_handle_t handle){
    return epoll_poller_->reset_out_event(handle);
}

uint32_t event_poller_t::load(){
    return epoll_poller_->load();
}
//////////////////////////////event_poller_t///////////////////////////////

event_loop_impl::event_loop_impl(){
    // create poller
    poller_ = new event_poller_t();
    // create thread weak up
    wake_up_ = new wake_up_pipe_t(this);
}

event_loop_impl::~event_loop_impl(){
    Z_DELETE_P(wake_up_);
    Z_DELETE_P(poller_);
}

event_poller_t* event_loop_impl::poller() const{
    return poller_;
}

void event_loop_impl::run(){
    while(1){
        // get poll timeout time and flush time after task
        int poll_time = execute_time_after_task();
        poller_->poll(poll_time);
    }
}

void event_loop_impl::async(Func&& func){
    // not in this thread insert to tasklist
    {
        std::lock_guard<std::mutex> lock(task_mtx_);
        async_tasks_.emplace_back(func);
    }
    // wake up poller
    wake_up_->wake_up();
}

void event_loop_impl::on_wake_up(){
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

void event_loop_impl::add_time_task(const shared_timer_task& task){
    z_time_t time_point = zpkg::times::now_ms() + task->time_after();

    async([time_point,task,this](){
        time_after_tasks_.emplace(time_point,task);
    });
}
int event_loop_impl::execute_time_after_task(){
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
            shared_timer_task task = it->second;
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
};