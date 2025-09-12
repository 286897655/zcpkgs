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

#include "epoll_poller.h"
#include <sys/epoll.h>
#include "uv_io_error.h"
#include <zlog/log.h>
#include "zio/io_utility.h"

namespace zio{

static epoll_handle create_epoll_handle()
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
            fd_control::make_close_on_exec(fd);
    }
    return fd;
}

epoll_poller* epoll_poller::create()
{
    epoll_handle handle = create_epoll_handle();
    if(handle == epoll_invalie_handle){
        return nullptr;
    }
    return new epoll_poller(handle);
}

epoll_poller::epoll_poller(epoll_handle handle):epoll_fd_(handle),load_(0){}

epoll_poller::~epoll_poller()
{
    if (epoll_fd_ != epoll_invalie_handle)
        ::close(epoll_fd_);
}

struct epoll_entity_t{
    io_fd_t fd;
    struct epoll_event e_event;
    poll_event_handler* poll_handler;
};

poll_handle_t epoll_poller::add_fd(io_fd_t fd,int poll_event,poll_event_handler* handler)
{
    // TODO 加一个判断是否在fd线程的操作
    //Z_ASSERT()
    epoll_entity_t* epoll_entity = new epoll_entity_t();
    epoll_entity->fd = fd;
    epoll_entity->poll_handler = handler;
    epoll_entity->e_event.events = ((poll_event & event_read ? EPOLLIN : 0) |
                                    (poll_event & event_write ? EPOLLOUT : 0) |
                                    (poll_event & event_ET ? EPOLLET : 0)) ;
    epoll_entity->e_event.data.ptr = epoll_entity;

    const int rc = epoll_ctl(epoll_fd_,EPOLL_CTL_ADD,fd,&epoll_entity->e_event);
    Z_ASSERT(rc != -1);

    // load increase
    load_ += 1;
    zlog("epoll {} add fd:{}",epoll_fd_,fd);
    return epoll_entity;
}

void epoll_poller::rm_fd(poll_handle_t handle)
{
    epoll_entity_t* epoll_entity = static_cast<epoll_entity_t*>(handle);
    const int rc = epoll_ctl(epoll_fd_,EPOLL_CTL_DEL,epoll_entity->fd,NULL);
    Z_ASSERT(rc != -1);
    epoll_entity->fd = invalid_io_fd_t;
    retired_.push_back(epoll_entity);

    // load decrease
    load_ -= 1;
}

void epoll_poller::set_in_event(poll_handle_t handle)
{
    epoll_entity_t* epoll_entity = static_cast<epoll_entity_t*>(handle);
    Z_ASSERT(epoll_entity->fd != invalid_io_fd_t);
    epoll_entity->e_event.events |= EPOLLIN;
    const int rc = epoll_ctl (epoll_fd_, EPOLL_CTL_MOD, epoll_entity->fd, &epoll_entity->e_event);
    Z_ASSERT (rc != -1);
}

void epoll_poller::reset_in_event(poll_handle_t handle)
{
    epoll_entity_t* epoll_entity = static_cast<epoll_entity_t*>(handle);
    Z_ASSERT(epoll_entity->fd != invalid_io_fd_t);
    epoll_entity->e_event.events &= ~(static_cast<uint32_t>(EPOLLIN));
    const int rc = epoll_ctl (epoll_fd_, EPOLL_CTL_MOD, epoll_entity->fd, &epoll_entity->e_event);
    Z_ASSERT (rc != -1);
}

void epoll_poller::set_out_event(poll_handle_t handle)
{
    epoll_entity_t* epoll_entity = static_cast<epoll_entity_t*>(handle);
    Z_ASSERT(epoll_entity->fd != invalid_io_fd_t);
    epoll_entity->e_event.events |= EPOLLOUT;
    const int rc = epoll_ctl (epoll_fd_, EPOLL_CTL_MOD, epoll_entity->fd, &epoll_entity->e_event);
    Z_ASSERT (rc != -1);
}

void epoll_poller::reset_out_event(poll_handle_t handle)
{
    epoll_entity_t* epoll_entity = static_cast<epoll_entity_t*>(handle);
    Z_ASSERT(epoll_entity->fd != invalid_io_fd_t);
    epoll_entity->e_event.events &= ~(static_cast<uint32_t>(EPOLLOUT));
    const int rc = epoll_ctl (epoll_fd_, EPOLL_CTL_MOD, epoll_entity->fd, &epoll_entity->e_event);
    Z_ASSERT (rc != -1);
}

uint32_t epoll_poller::load(){
    return load_;
}

void epoll_poller::poll(int timeout)
{
    epoll_event events[epoll_max_io_events];
    const int event_count = epoll_wait(epoll_fd_,&events[0],epoll_max_io_events,timeout ? timeout : -1);
    if(event_count <= 0){
        return;
    }
    for (int i = 0; i < event_count; i++) {
        epoll_entity_t* epoll_entity = static_cast<epoll_entity_t*>(events[i].data.ptr);
        struct epoll_event &epoll_event = events[i];
        
        if(!epoll_entity || !epoll_entity->poll_handler)
            continue;
        if(epoll_entity->fd == invalid_io_fd_t){
            // delete fd ,ignore
            continue;
        }
        if(epoll_event.events & (EPOLLERR | EPOLLHUP)){
            // poll error for in event to read and got error
            // then delete or process other 
            epoll_entity->poll_handler->in_event();
        }
        // may be delete in error event then ignore again
        if(epoll_entity->fd == invalid_io_fd_t){
            continue;
        }
        if(epoll_event.events & (EPOLLOUT)){
            // poll out for out event
            epoll_entity->poll_handler->out_event();
        }
        // may be delete in out event then ignore again
        if(epoll_entity->fd == invalid_io_fd_t){
            continue;
        }
        if(epoll_event.events & (EPOLLIN)){
            // poll in for in event
            epoll_entity->poll_handler->in_event();
        }
    }
    // delete retired poll handle
    for(auto var : retired_){
        Z_DELETE_P(var);
    }
    retired_.clear();
}

}