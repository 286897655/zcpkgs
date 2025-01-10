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

#include "zio/io_utility.h"
#include <zpkg/utility.h>
#include <fcntl.h>

namespace zio{

void fd_control::make_non_blocking(io_fd_t fd){
#ifdef Z_SYS_WINDOWS
    u_long nonblock = 1;
    const int rc = ioctlsocket (fd, FIONBIO, &nonblock);
#elif Z_SYS_LINUX
    int flags = ::fcntl (fd, F_GETFL, 0);
    if (flags < 0)
        flags = 0;
    const int rc = ::fcntl (fd, F_SETFL, flags | O_NONBLOCK);
#endif
    Z_ASSERT(rc >= 0);
}

void fd_control::make_close_on_exec(io_fd_t fd){
#ifdef Z_SYS_LINUX
    int flags = fcntl(fd, F_GETFD);
    if(flags < 0){
        flags =0;
    }
    const int rc = ::fcntl(fd, F_SETFD, flags | FD_CLOEXEC);
    Z_ASSERT(rc >= 0);
#endif
}

};//!namespace zio