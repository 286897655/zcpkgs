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

#ifndef ZPKG_SYS_H_
#define ZPKG_SYS_H_

#include <string>

namespace zpkg{

/**
* get system page size
*/
size_t sys_getpagesize();

// wrapper for application set for systems
std::string sys_app_dir();
std::string sys_app_path();
std::string sys_app_name();

// wrapper for popen/pclose for systems
FILE* sys_popen(const std::string& command, const std::string& mode);
int sys_pclose(FILE* stream);

/**
 * @brief run as daemon service for linux fork 
 * 
 */
void sys_run_daemon();

/**
 * @brief set current thread name
 * 
 */
void sys_set_curtr_name(const std::string& name);

/**
 * @brief Set the Limit(core and fd limit) And Sig Crash
 * 
 */
void sys_set_limit_and_crash_sig();

void sys_write_syslog(const std::string& log);

/**
 * @brief execute a command
 * 
 * @param command 
 * @return std::string 
 */
std::string sys_exec_cmd(const std::string& command);

namespace sys{

/// @brief class for single process mutex
class process_mutex{
public:
    process_mutex();
    ~process_mutex();
public:
    void lock();
    void unlock();

private:
    int lock_fd;
};

};//!namesapce sys

};//!namespace zpkg

#endif //!ZPKG_SYS_H_