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

#ifndef ZIO_IO_POLLER_H_
#define ZIO_IO_POLLER_H_

#include <string>
#include <functional>
#include <thread>

#if defined _WIN32
// Windows uses a pointer-sized unsigned integer to store the socket fd.
#if defined _WIN64
typedef unsigned __int64 zio_fd_t;
#else
typedef unsigned int zio_fd_t;
#endif
#else
typedef int zio_fd_t;
#endif

namespace zio{
class io_poller_t;

class io_loop_t{
public:
    // 在主线程中先调用一次
    static io_loop_t* default_loop();
    static io_loop_t* next_loop();
    static io_loop_t* min_load_loop();
    static io_loop_t* this_thread_loop();
    static size_t create_loop_pool(const std::string& name="",size_t count = 0);
    
public:
    void async(std::function<void()>&& callback);
    void run();
    bool is_this_thread_loop() const;
    io_poller_t* poller() const;

private:
    explicit io_loop_t(const std::string& loop_name);
    ~io_loop_t();

    // run in loop thread
    void init();

    std::string loop_name_;
    std::thread* loop_thread_;
    io_poller_t* loop_poller_;

    //std::multimap<uint64_t, timer_info_t> timers_t;
};

class io_handler_t{

};

class epoll_poller;
class io_poller_t{
public:
    io_poller_t();


private:
    epoll_poller* epoll_poller_;


};

}//!namespace zio

#endif//!ZIO_IO_POLLER_H_