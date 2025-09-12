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

#include "io_ctx.h"
#include "zio/io_loop_impl.h"
#include "zio/epoll_poller.h"
#include <zpkg/memory.h>
#include <zlog/log.h>
namespace zio{

class io_loop_pool{
public:
    io_loop_pool()=default;
    ~io_loop_pool()=default;

    void create(size_t count){
        for(size_t i = 0;i < count;i++){
            std::string thread_name = "io loop " + std::to_string(i);
            threads_.emplace_back(std::make_unique<std::thread>([this,thread_name](){
                io_loop_t* loop = io_loop_t::create_this_thread_loop();
                loops_.emplace_back(loop);
                loop->set_loop_name(thread_name);
                loop->run_loop();
            }));
        }
    };

private:
    std::vector<std::unique_ptr<std::thread>> threads_;
    std::vector<io_loop_t*> loops_;
};

///////////////////// static field //////////////////////////////
static io_loop_t* default_loop_ptr = nullptr;
static thread_local io_loop_t* tls_loop_ptr = nullptr;
static io_loop_pool* loop_pool_ = nullptr;
///////////////////// static field //////////////////////////////

io_loop_t* io_loop_t::create_this_thread_loop(){
    Z_ASSERT(!tls_loop_ptr);
    tls_loop_ptr = new io_loop_t();

    return tls_loop_ptr;
}

// static io_loop_t* default_loop();
io_loop_t* io_loop_t::default_loop(){
    if(default_loop_ptr)
        return default_loop_ptr;
    // no default_loop create default loop and bind tls
    static std::once_flag default_once_flag;
    std::call_once(default_once_flag,[](){
        default_loop_ptr = create_this_thread_loop();
    });
    Z_ASSERT(default_loop_ptr);
    return default_loop_ptr;
}

// static io_loop_t* this_thread_loop();
io_loop_t* io_loop_t::this_thread_loop(){
    return tls_loop_ptr;
}

// static size_t create_loop_pool(size_t count = 0);
size_t io_loop_t::create_loop_pool(size_t count){
    count = count > 0 ? count : std::thread::hardware_concurrency();
    static std::once_flag pool_once_flag;
    std::call_once(pool_once_flag,[count](){
        Z_ASSERT(!loop_pool_);
        loop_pool_ = new io_loop_pool();
        loop_pool_->create(count);
    });
    Z_ASSERT(loop_pool_);
    return count;
}

// default constructor
io_loop_t::io_loop_t():loop_impl_(new io_loop_impl()){

}

io_loop_t::~io_loop_t(){
    delete loop_impl_;
    loop_impl_=nullptr;
}

void io_loop_t::async(Func&& callback){
    if(is_this_thread_loop()){
        callback();
        return;
    }

    loop_impl_->async(std::move(callback));
}

void io_loop_t::sync(Func&& func){
    
}

void io_loop_t::add(Func&& func){
    return async(std::move(func));
}

int io_loop_t::run_loop(){
    return loop_impl_->run();
}

bool io_loop_t::is_this_thread_loop() const{
    return tls_loop_ptr == this;
}

io_poller_t* io_loop_t::poller() const{
    return loop_impl_->poller();
}

uint32_t io_loop_t::load(){
    return loop_impl_->poller()->load();
}

void io_loop_t::set_loop_name(const std::string name){
    async([name](){
        pthread_setname_np(pthread_self(),name.c_str());
    });
}

////////////////////////io_poller_t//////////////////////////////////////
io_poller_t::io_poller_t():epoll_poller_(epoll_poller::create())
{   
    
}

io_poller_t::~io_poller_t()
{

}

void io_poller_t::poll(int timeout)
{
    epoll_poller_->poll(timeout);
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
////////////////////////io_poller_t//////////////////////////////////////
////////////////////////io_timer_t//////////////////////////////////////

io_timer_t::time_after_task::time_after_task(z_time_t after_msec,std::function<bool(void)>&& boolean_cb)
    :after_msec_(after_msec),boolean_cb_(boolean_cb),canceled_(false){

}

bool io_timer_t::time_after_task::call() const{
    if(!canceled_){
        return boolean_cb_();
    }
    return false;
}
void io_timer_t::time_after_task::cancel(){
    canceled_.store(true);
}

io_timer_t::io_timer_t(io_loop_t* loop):loop_impl_(loop->loop_impl_){
}

io_timer_t::~io_timer_t(){
    if(auto strong_call = weak_call_.lock()){
        strong_call->cancel();
        weak_call_.reset();
    }
}

// 返回值false表示不再重复触发，返回值true表示重复触发
void io_timer_t::reset(boolean_call_back&& cb,z_time_t msec_interval){
    if(auto strong_call = weak_call_.lock()){
        strong_call->cancel();
        weak_call_.reset();
    }

    auto task = std::make_shared<time_after_task>(msec_interval,std::move(cb));
    weak_call_ = task;
    loop_impl_->add_time_task(task);
}

// 取消定时器
void io_timer_t::cancel(){
    if(auto strong_call = weak_call_.lock()){
        strong_call->cancel();
        weak_call_.reset();
    }
}
////////////////////////io_timer_t//////////////////////////////////////

};//!namespace zio