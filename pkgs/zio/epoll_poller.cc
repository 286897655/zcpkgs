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

#include "epoll_poller.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include "uv_io_error.h"

namespace zio{

epoll_handle epoll_poller::create_epoll_handle()
{
#if defined(EPOLL_CLOEXEC)
    epoll_handle fd = epoll_create1(EPOLL_CLOEXEC);
#else // defined(EPOLL_CLOEXEC)
    epoll_handle fd = epoll_invalie_handle;
    errno = EINVAL;
#endif // defined(EPOLL_CLOEXEC)
    if (fd == epoll_invalie_handle && (errno == EINVAL || errno == ENOSYS))
    {
        fd = epoll_create(epoll_max_io_events);
        if (fd != -1)
            ::fcntl(fd, F_SETFD, FD_CLOEXEC);
    }
    return fd;
}

epoll_poller::epoll_poller(epoll_handle handle):epoll_fd_(handle)
{

}

epoll_poller::~epoll_poller()
{
    if (epoll_fd_ != epoll_invalie_handle)
        ::close(epoll_fd_);
}

void epoll_poller::add_fd(zio_fd_t fd,io_handler_t* handler)
{
    struct epoll_event ev = {0};
    ev.events = toEpoll(event) ;
    ev.data.fd = fd;
    const int rc = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev);
}

void epoll_poller::rm_fd(zio_fd_t fd)
{
    const int rc = epoll_ctl(epoll_fd_, EPOLL_CTL_DEL,fd,NULL);
}

void epoll_poller::set_in_event(zio_fd_t fd)
{
    struct epoll_event ev = {0};
    ev.events = toEpoll(event) ;
    ev.data.fd = fd;
}

void epoll_poller::reset_in_event(zio_fd_t fd)
{

}

void epoll_poller::set_out_event(zio_fd_t fd)
{

}

void epoll_poller::reset_out_event(zio_fd_t fd)
{

}

uint32_t epoll_poller::load()
{

}

void epoll_poller::poll(int timeout)
{
    epoll_event events[epoll_max_io_events];
    const int event_count = epoll_wait(epoll_fd_,&events[0],epoll_max_io_events,timeout ? timeout : -1);
    if(event_count <= 0){
        return;
    }

    
}

}