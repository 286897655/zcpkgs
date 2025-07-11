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
#ifndef ZPKG_ZLOG_H_
#define ZPKG_ZLOG_H_

#include <memory>
#include <vector>
#include <zpkg/zlog_fmt_inl.h>

/**
 * sync log
 * work-thread1  work-thread2 .... work-threadn
 *      |           |                   |
 *      |--------mutex lock-------------|
 *                  |
 *                  |
 *                  |
 * sink1          sink2...           sink-n
*/

/**
 * async log
 * work-thread1  work-thread2 .... work-threadn
 *      |           |                   |
 *  spsc-queue1  spsc-queue2  ....  spsc-queuen
 *      |           |                   |
 *      |--------log thread-------------|
 *                  |
 * sink1          sink2...           sink-n
*/

/**
 * 实现一个支持异步的日志库
 * 想要实现的目标：
 * 默认什么都不调用，直接调用zlog写日志的时候相当于调用fprintf输出到控制台
 * 
 * 拆解：同步日志+异步日志
 * 同步日志：调用阻塞到日志输出
 * 异步日志：调用非阻塞，异步线程输出日志
 * 
 * 输出到控制台一个sink
 * rotatefile 一个sink
 * 日志可以回调一个sink
 * 
 * sink可以挂载到同步日志或者异步日志中
 * 在sink里面设置需要log的级别
 * 
 * 要支持异步日志，format就得在log调用的时候format好
 * 
 * 
 * 
*/
enum{
    ZLOG_LEVEL_DEBUG = 0,
    ZLOG_LEVEL_INFO  = 1,
    ZLOG_LEVEL_WARN  = 2,
    ZLOG_LEVEL_ERROR = 3
};
namespace zlog{

class logger;

/**
 * zlog level enum
 * debug
 * info
 * warn
 * error
*/
enum class zlog_level{
    debug = ZLOG_LEVEL_DEBUG,
    info = ZLOG_LEVEL_INFO,
    warn = ZLOG_LEVEL_WARN,
    error = ZLOG_LEVEL_ERROR
};

/**
 * source file function line information for debug
*/
struct log_source_ffl{
    const char* file = nullptr;
    const char* func = nullptr;
    int line = -1;

    log_source_ffl() = default;

    constexpr log_source_ffl(const char* filename,const char* function,int fileline)
        :file(filename),func(function),line(fileline){}

    constexpr bool none() const {
        return line <= 0;
    }
};

/**
 * 用于创建logger对象的全局管理类 单例实例
 * 
 * log service used for manage logger,an single instance
*/
class log_service{
public:
    // 返回logger管理的单例实例
    // return single instance of log_service
    static log_service* service();
    
    /// @brief 获取当前的默认logger
    /// 当前只能存在唯一的一个logger
    /// 要么是sync的,要么是async的,不会同时存在
    /// 初始调用没有logger会默认创建一个sync 挂载console sink的logger
    /// return default logger
    /// @return 
    logger* default_logger() const{
        return default_logger_;
    }

private:
    log_service();

private:
    logger* default_logger_;
};

// hold msg buffer
class log_msg{

};

// just view of log msg
class log_msg_view{

};

class log_sink : std::enable_shared_from_this<log_sink>{
public:
    enum type{
        console,
        file,
        callback
    };

public:
    explicit log_sink(type sink_type);
    ~log_sink();

    void set_log_level(zlog_level level);

};

class logger{
public:
    /// @brief 创建或获取默认的sync logger
    /// @return 
    static logger* create_or_get_sync_logger();
    /// @brief 创建或获取默认的async logger
    /// @return 
    static logger* create_or_get_async_logger(/*option*/);

    /// @brief create or get console sink
    ///        console sink is default sink
    /// @return 
    std::shared_ptr<log_sink> create_or_get_console_sink();
    void remove_console_sink();

    std::shared_ptr<log_sink> create_or_get_file_sink(const std::string& sink_id/*option*/);
    void remove_file_sink(const std::string& sink_id);
    std::shared_ptr<log_sink> create_or_get_cb_sink(const std::string& sink_id/*cb*/);
    void remove_cb_sink(const std::string& sink_id);

    template<typename... Args>
    inline void log(zlog_level level,const log_source_ffl& sffl,format_string_t<Args...> fmt, Args &&...args){
        
    };

private:
    logger();

    inline void log_l();

private:
    std::vector<std::shared_ptr<log_sink>> sinks_;
};


inline void debug(){
    //log_service::service()->default_logger()->log(zlog_level::debug,log_source_ffl{},);
}
inline void print(){
    //return info();
}
inline void info(){

}
inline void warn(){

}
inline void error(){

}

}//!namespace zlog

// define zlog()
// define zlog_debug()
// define zlog_info()
// define zlog_warn()
// define zlog_error()

#endif //!ZPKG_ZLOG_H_