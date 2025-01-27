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

#include "zpkg/fsutil.h"
#include "zpkg/sys.h"
#include "zlog/log.h"
#include "zpkg/strings.h"
#include <fstream>
#include <sstream>

#ifdef Z_SYS_LINUX
#include <sys/stat.h>
#include <sys/dir.h>
#endif

namespace zpkg{

std::string fs_read_content(const std::string& filename)
{
    std::ifstream ifs(filename);

    if(ifs.is_open()){
        std::ostringstream oss;
        oss << ifs.rdbuf();

        return oss.str();
    }

    return "";
}

size_t fs_read_binsize(const std::string& filename)
{
    FILE* rfile = fopen(filename.c_str(), "rb");
    if (!rfile) return 0;

    // seek to end
    fseek(rfile, 0, SEEK_END);
    // tell current pos
    size_t size = ftell(rfile);
    fclose(rfile);

    return size;
}

namespace fs{

bool Path::Exist(const std::string& path){
#ifdef Z_SYS_WINDOWS
    DWORD attribs = ::GetFileAttributes(path.c_str());
    return attribs != INVALID_FILE_ATTRIBUTES;
#else // common linux/unix all have the stat system call
    struct stat st;
    return (::stat(path.c_str(), &st) == 0);
#endif
}

void Path::RecursiveCreate(const std::string& path){
    if(Exist(path))
        return;
    
    // path to remove back slash
    std::string create_path = path;
    if(create_path.back() == '/'){
        create_path.pop_back();
    }

    // do not exist
    // create parent
    size_t pos = create_path.rfind('/');
    if(pos != std::string::npos){
        std::string parent = create_path.substr(0,pos);
        RecursiveCreate(parent);
    }

    // no find or create parent success
    // create current
#ifdef Z_SYS_WINDOWS
    if (::_mkdir(create_path.c_str()) < 0) {
#else
    constexpr mode_t mode = S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IXOTH;
    if (::mkdir(create_path.c_str(), mode) == -1) {
#endif
        zlog_error("Path create fail:{},error:{}",create_path,errno);
    }
}

std::string Path::Relative2Absolute(const std::string& relative,const std::string& current)
{
    std::string currentPath = current;
    if(currentPath.empty()){
        currentPath = sys_app_dir();
    }
    else{
        // currentPath is not empty maybe it'e a relativepath
        if(currentPath.front() == '.'){
            currentPath = Relative2Absolute(currentPath,sys_app_dir());
        }
    }

    if(currentPath.back() != '/'){
        currentPath.push_back('/');
    }

    if(relative.empty()){
        return currentPath;
    }

    std::vector<std::string> path_split = strings::split(relative,"/");
    for (auto &dir : path_split) {
        if (dir.empty() || dir == ".") {
            // empty or this path
            continue;
        }
        if (dir == "..") {//parent
            currentPath = ParentPath(currentPath);
            continue;
        }
        currentPath.append(dir);
        currentPath.append("/");
    }

    if (relative.back() != '/' && currentPath.back() == '/') {
        //在路径是文件的情况下，防止转换成目录
        currentPath.pop_back();
    }
    return currentPath;
}

std::string Path::ParentPath(const std::string& currentPath)
{
    std::string current = currentPath;
    if(currentPath.empty()){
        current = sys_app_dir();
    }

    if(current.back() == '/'){
        current.pop_back();
    }

    size_t pos = current.rfind('/');
    if(pos != std::string::npos){
        current = current.substr(0,pos+1);
    }

    return current;
}


CFileHandle::CFileHandle():_handle(nullptr)
{

}

CFileHandle::~CFileHandle()
{
    if(_handle){
        int ret = std::fclose(_handle);
        if(ret < 0){
            zlog_error("CFileHandle fclose {},it will be leaked",fmt::ptr(_handle));
        }
    }
}

std::shared_ptr<CFileHandle> File::Create(const std::string& filewithpath,const char* mode)
{
    if(filewithpath.empty()){
        zlog_warn("File Create empty filename.");
        return nullptr;
    }

    // check error parameter
    if((filewithpath.back() == '/')){
        zlog_warn("File Create ({}) error file with path.",filewithpath);
        return nullptr;
    }

    std::string filename = filewithpath;
    if(filename.find('/') == std::string::npos){//no path spcific
        filename = sys_app_dir() + filename;
    }

    if(filename.front() == '.'){//maybe relative path
        filename = Path::Relative2Absolute(filename,"");
    }

    if(filename.front() != '/'){
        zlog_warn("File Create ({}) error file with path.",filewithpath);
        return nullptr;
    }
        
    std::string path = filename.substr(0,filename.rfind('/'));
    Path::RecursiveCreate(path);

    // open file
    std::FILE* io_file = ::fopen(filename.c_str(),mode);
    if(!io_file){
        zlog_error("open file {} mode {} error {}.",filename,mode,errno);
        return nullptr;
    }

    // open success
    std::shared_ptr<CFileHandle> file_handle = std::make_shared<CFileHandle>();
    file_handle->_handle = io_file;
    file_handle->_filename = filename;
    return file_handle;
}

bool File::Exists(const std::string& filewithpath)
{
    std::ifstream ftest(filewithpath);
    return ftest.good();
}

Dir::Dir(const std::string& path)
{

}

Dir::~Dir()
{
    
}

std::vector<std::string> Dir::FileList()
{
    return std::vector<std::string>();
}

std::vector<std::string> Dir::GetFileList(const std::string& path)
{
    std::vector<std::string> files;
    
#if __cplusplus >= 201703L
    for(const auto& file : std::filesystem::directory_iterator(path)){
        files.push_bakc(file);
    }
#else
#ifdef Z_SYS_LINUX
    ::DIR *dir = NULL; struct dirent *diread = NULL;
    if((dir = opendir(path.c_str())) != NULL){
        while((diread = readdir(dir)) != NULL){
            if(diread->d_type != DT_REG){// only regualler file
                continue;
            }
            files.push_back(diread->d_name);
        }
        closedir(dir);
    }
#endif
#endif
    return files;
}

};//!namespace fs
};//!namespace zpkg