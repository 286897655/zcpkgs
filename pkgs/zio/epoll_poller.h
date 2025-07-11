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

#ifndef ZIO_EPOLL_POLLER_H_
#define ZIO_EPOLL_POLLER_H_

#include <atomic>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "zio/io_loop_ctx.h"

namespace zio{

using epoll_handle = int;

enum{
    // invalid epoll handle defind
    epoll_invalie_handle = -1,
    // max io epoll events in one loop
    epoll_max_io_events = 1024
};

struct epoll_entity_t;

class epoll_poller{
public:
    static epoll_poller* create();
private:
    explicit epoll_poller(epoll_handle handle);
public:
    ~epoll_poller();

    poll_handle_t add_fd(io_fd_t fd,int poll_event,poll_event_handler* handler);
    void rm_fd(poll_handle_t handle);
    void set_in_event(poll_handle_t handle);
    void reset_in_event(poll_handle_t handle);
    void set_out_event(poll_handle_t handle);
    void reset_out_event(poll_handle_t handle);
    uint32_t load();

    void poll(int timeout = -1);

private:
    
private:
    epoll_handle epoll_fd_;
    uint32_t load_;
    std::vector<struct epoll_entity_t*> retired_;
    Z_DISABLE_COPY_MOVE(epoll_poller)
};




};



#endif //!ZIO_EPOLL_POLLER_H_