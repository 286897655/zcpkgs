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
#include "zio/io_poller.h"
#include <zpkg/utility.h>
#include <zlog/log.h>
#include "epoll_poller.h"
#include "wake_up_pipe.h"

namespace zio{

static io_loop_t* default_loop_ptr_ = nullptr;

thread_local io_loop_t* thread_loop = nullptr;

// 在 主线程一定运行过一次
io_loop_t* io_loop_t::default_loop()
{
    if(default_loop_ptr_)  return default_loop_ptr_;

    default_loop_ptr_ = new io_loop_t("default loop");
    default_loop_ptr_->init();
    thread_loop = default_loop_ptr_;
    return default_loop_ptr_;
}

io_loop_t* io_loop_t::next_loop()
{
    return nullptr;
}

io_loop_t* io_loop_t::min_load_loop()
{
    return nullptr;
}

size_t io_loop_t::create_loop_pool(const std::string& name,size_t count){
    auto default_loop = io_loop_t::default_loop();
    if(!default_loop){
        throw std::runtime_error("first create default loop fail");
    }
    // set default name and default thread count
    std::string thread_name = name.empty()?"io loop":name;
    count = count > 0 ? count : std::thread::hardware_concurrency();
    for(size_t i = 1 ; i <= count ; i++){
        std::string loop_name = thread_name + " " + std::to_string(i);
        io_loop_t* loop = new io_loop_t(loop_name);
        loop->loop_thread_ = new std::thread([loop](){
            loop->init();
            loop->run();
        });
    }
    return count;
}

io_loop_t::io_loop_t(const std::string& loop_name):loop_name_(loop_name),loop_thread_(nullptr)
{

}

io_loop_t::~io_loop_t()
{

}

void io_loop_t::init()
{
    // set thread name
    pthread_setname_np(pthread_self(),loop_name_.c_str());

    // create poller
    loop_poller_ = new io_poller_t();
    zlog("zio:created io_loop {}",loop_name_);
    // bind thread loop
    thread_loop = this;
}

void io_loop_t::async(call_back_func&& callback)
{
    if(is_this_thread_loop()){

        callback();
        return;
    }
    // not in this thread insert to tasklist
    {
        std::lock_guard<std::mutex> lock(task_mtx_);
        async_tasks_.emplace_back(callback);
    }
    // wake up poller
    wake_up_->wake_up();
}

void io_loop_t::on_wake_up(){
    Z_ASSERT(is_this_thread_loop());

    // flush task's
    std::vector<call_back_func> swap_task;
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

bool io_loop_t::is_this_thread_loop() const
{
    return thread_loop == this;
}

void io_loop_t::run()
{
    while(1){
        loop_poller_->poll();
    }
}


io_poller_t::io_poller_t():epoll_poller_(epoll_poller::create())
{   
    
}

io_poller_t::~io_poller_t()
{

}

void io_poller_t::poll()
{
    epoll_poller_->poll();
}

poll_handle_t io_poller_t::add_fd(io_fd_t fd,int poll_event,poll_event_handler* handler)
{
    return epoll_poller_->add_fd(fd,poll_event,handler);
}

void io_poller_t::rm_fd(poll_handle_t handle)
{
    return epoll_poller_->rm_fd(handle);
}

void io_poller_t::set_in_event(poll_handle_t handle)
{
    return epoll_poller_->set_in_event(handle);
}

void io_poller_t::reset_in_event(poll_handle_t handle)
{
    return epoll_poller_->reset_in_event(handle);
}

void io_poller_t::set_out_event(poll_handle_t handle)
{
    return epoll_poller_->set_out_event(handle);
}

void io_poller_t::reset_out_event(poll_handle_t handle)
{
    return epoll_poller_->reset_out_event(handle);
}

uint32_t io_poller_t::load()
{
    return epoll_poller_->load();
}

}