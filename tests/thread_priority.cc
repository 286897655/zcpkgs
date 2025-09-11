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

#include <pthread.h>
#include <zlog/log.h>

#include <sched.h>
#include <sys/resource.h>
#include <sys/syscall.h>
# define gettid() syscall(SYS_gettid)

int main(int arc,char** argv){
    zlog::logger::create_defaultLogger();

    int min_priority = sched_get_priority_min(SCHED_OTHER);
    int max_priority = sched_get_priority_max(SCHED_OTHER);

    zlog("thread min:{} ---- max:{}",min_priority,max_priority);


    std::thread back_thread([](){
        zlog("run in current thread:{}",gettid());
        zlog("current thread prio:{}",getpriority(PRIO_PROCESS,gettid()));

        zlog("set thread prio:-10");
        zlog("set thread prio result:{}",setpriority(PRIO_PROCESS,gettid(),-10));
        while(1){
            sleep(2);
        }
    });
    back_thread.join();
};