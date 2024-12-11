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
#include "zio/io_poller.h"
#include <zpkg/utility.h>
#include <zlog/log.h>
#include "epoll_poller.h"

namespace zio{

static io_loop_t* default_loop_ptr_ = nullptr;

thread_local io_loop_t* thread_loop = nullptr;

// 在 抓线程一定运行过一次
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

}

io_loop_t* io_loop_t::min_load_loop()
{

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
    loop_poller_ = new epoll_poller();
    zlog("zio:created io_loop {}",loop_name_);
    // bind thread loop
    thread_loop = this;
}

void io_loop_t::async(std::function<void()>&& callback)
{

}

void io_loop_t::run()
{
    while(1){
        // test
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


}