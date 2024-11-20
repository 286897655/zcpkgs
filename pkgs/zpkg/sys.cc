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

#include "zpkg/sys.h"
#include "zpkg/portable.h"
#ifdef Z_SYS_WINDOWS
#include <windows.h>
#define PATH_MAX MAX_PATH
#elif defined(Z_SYS_MACH)
#error "not support now"
#elif defined(Z_SYS_LINUX)
#include <limits.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/syslog.h>
#include <execinfo.h>
#endif

namespace zpkg{
namespace sys{

std::string app_dir()
{
    std::string path = app_path();
    return path.substr(0, path.rfind('/') + 1);
}

std::string app_path()
{
    char buffer[PATH_MAX * 2 + 1] = {0};
    int n = -1;
#if defined(Z_SYS_WINDOWS)
    n = GetModuleFileNameA(NULL, buffer, sizeof(buffer));
#elif defined(Z_SYS_LINUX)
    n = readlink("/proc/self/exe", buffer, sizeof(buffer));
#endif

    std::string filePath;
    if (n <= 0) {
        filePath = "./";
    } else {
        filePath = buffer;
    }

#if defined(Z_SYS_WINDOWS)
    //windows->unix
    for (auto &ch : filePath) {
        if (ch == '\\') {
            ch = '/';
        }
    }
#endif //defined(_WIN32)

    return filePath;
}

std::string app_name()
{
    std::string path = app_path();
    return path.substr(path.rfind('/') + 1);
}

};//!namespace sys
};//!namespace zpkg


