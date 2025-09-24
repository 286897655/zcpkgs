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

#include "events.h"
#include <zpkg/utility.h>
#include <thread>
#include "event_loop_impl.h"

namespace zio{
class event_loop_pool{
public:
    event_loop_pool()=default;
    ~event_loop_pool()=default;

    void create(size_t count){
        for(size_t i = 0;i < count;i++){
            std::string thread_name = "event loop " + std::to_string(i);
            threads_.emplace_back(std::make_unique<std::thread>([this,thread_name](){
                event_loop_t* loop = events_ctx::create_this_thread_loop();
                loops_.emplace_back(loop);
                loop->set_loop_name(thread_name.c_str());
                loop->run_loop();
            }));
        }
    };

private:
    std::vector<std::unique_ptr<std::thread>> threads_;
    std::vector<event_loop_t*> loops_;
};

///////////////////// static field //////////////////////////////
static event_loop_t* default_loop_ptr = nullptr;
static thread_local event_loop_t* tls_loop_ptr = nullptr;
static event_loop_pool* loop_pool_ptr = nullptr;
///////////////////// static field //////////////////////////////

event_loop_t* events_ctx::default_loop(){
    if(default_loop_ptr)
        return default_loop_ptr;
    
    // no default_loop create default loop and bind tls
    static std::once_flag default_once_flag;
    std::call_once(default_once_flag,[](){
        default_loop_ptr = create_this_thread_loop();
    });

    return default_loop_ptr;
}

event_loop_t* events_ctx::create_this_thread_loop(){
    Z_ASSERT(!tls_loop_ptr);
    tls_loop_ptr = new event_loop_t();
    return tls_loop_ptr;
}

event_loop_t* events_ctx::this_thread_loop(){
    return tls_loop_ptr;
}

size_t events_ctx::create_loop_pool(size_t count){
    count = count > 0 ? count : std::thread::hardware_concurrency();
    static std::once_flag pool_once_flag;
    std::call_once(pool_once_flag,[count](){
        Z_ASSERT(!loop_pool_ptr);
        loop_pool_ptr = new event_loop_pool();
        loop_pool_ptr->create(count);
    });
    Z_ASSERT(loop_pool_ptr);
    return count;
}

event_loop_t::event_loop_t():loop_impl_(new event_loop_impl()){
}

event_loop_t::~event_loop_t(){
    delete loop_impl_;
    loop_impl_ = nullptr;
}

event_poller_t* event_loop_t::poller() const{
    return loop_impl_->poller();
}
void event_loop_t::run_loop() const{
    return loop_impl_->run();
}
bool event_loop_t::is_this_thread_loop() const{
    return this == tls_loop_ptr;
}

uint32_t event_loop_t::load() const{
    return loop_impl_->poller()->load();
}

void event_loop_t::set_loop_name(const char* name){
    std::string loop_name(name);
    async([loop_name](){
        pthread_setname_np(pthread_self(),loop_name.c_str());
    });
}

void event_loop_t::add(Func&& func){
    async(std::move(func));
}
void event_loop_t::async(Func&& func){
    if(is_this_thread_loop()){
        func();
        return;
    }

    return loop_impl_->async(std::move(func));
}
void event_loop_t::sync(Func&& func){
    // TODO zhaoj use async now
    async(std::move(func));
}

////////////////////////event_timer_t//////////////////////
time_after_task::time_after_task(z_time_t after_msec,boolean_call_back&& boolean_cb)
    :after_msec_(after_msec),boolean_cb_(boolean_cb),canceled_(false){

}

bool time_after_task::call() const{
    if(!canceled_){
        return boolean_cb_();
    }
    return false;
}

void time_after_task::cancel(){
    canceled_.store(true);
}

event_timer_t::event_timer_t(event_loop_t* loop):loop_impl_(loop->loop_impl_){
}

event_timer_t::~event_timer_t(){
    if(auto strong_call = weak_call_.lock()){
        strong_call->cancel();
        weak_call_.reset();
    }
}

// 返回值false表示不再重复触发，返回值true表示重复触发
void event_timer_t::reset(boolean_call_back&& cb,z_time_t msec_interval){
    if(auto strong_call = weak_call_.lock()){
        strong_call->cancel();
        weak_call_.reset();
    }

    auto task = std::make_shared<time_after_task>(msec_interval,std::move(cb));
    weak_call_ = task;
    loop_impl_->add_time_task(task);
}

// 取消定时器
void event_timer_t::cancel(){
    if(auto strong_call = weak_call_.lock()){
        strong_call->cancel();
        weak_call_.reset();
    }
}
////////////////////////event_timer_t//////////////////////

};//!namespace zio