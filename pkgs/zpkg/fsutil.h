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

#ifndef ZPKG_FS_UTIL_H_
#define ZPKG_FS_UTIL_H_

#include <zpkg/utility.h>
#include <string>
#include <vector>
#include <stdio.h>

namespace zpkg{

std::string fs_read_content(const std::string& filename);

size_t fs_read_binsize(const std::string& filename);

namespace fs{

class Path{
public:
    static bool Exist(const std::string& path);

    static void RecursiveCreate(const std::string& path);

    /**
     * @brief 
     * 
     * @param relative 
     * @param current (empty current use exe dir)
     * @return std::string 
     */
    static std::string Relative2Absolute(const std::string& relative,const std::string& current);

    static std::string ParentPath(const std::string& currentPath);
};


// TODO zhaoj
// 用来持有FILE*指针，
// 希望添加支持获取该指针指向的文件
class CFileHandle : public std::enable_shared_from_this<CFileHandle>{
public:
    explicit CFileHandle();
    ~CFileHandle();
public:
    std::FILE* Handle() const{
        return _handle;
    }

    std::string HandleFileName() const{
        return _filename;
    }
protected:
    std::FILE* _handle;
    std::string _filename;
    friend class File;
};

class File{
public:
    /**
     * @brief create file and open it to write
     * if path is not exist,we will create path recursively
     * 
     * @param filewithpath 
     * @param mode 
     * @return std::shared_ptr<CFileHandle> 
     */
    static std::shared_ptr<CFileHandle> Create(const std::string& filewithpath,const char* mode);

    /**
     * @brief whether file is exists in path
     * 
     * @param filewithpath
     * @return true 
     * @return false 
     */
    static bool Exists(const std::string& filewithpath);
};

class Dir final{
public:
    Dir(const std::string& path);
    ~Dir();

    std::vector<std::string> FileList();
    static std::vector<std::string> GetFileList(const std::string& path);
private:
    Z_DISABLE_COPY_MOVE(Dir);
};

};//!namespace fs
};//!namespace zpkg


#endif //!ZPKG_FS_UTIL_H_