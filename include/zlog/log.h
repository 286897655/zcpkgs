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

#ifndef ZLOG_LOG_H_
#define ZLOG_LOG_H_

#include <atomic>
#include <zpkg/utility.h>
#include <spdlog/spdlog.h>

namespace zlog{
// an async logger
enum class log_level{
    LOGLEVEL_TRACE,
    LOGLEVEL_DEBUG,
    LOGLEVEL_INFO,
    LOGLEVEL_WARN,
    LOGLEVEL_ERROR
};

using log_callback = std::function<void(const std::string& log_msg)>;
/**
 * @brief zcf log use spdlog backend
 * 
 * 1、default use console logger
 * 2、or create rotate file sink logger
 * 3、or create asyn rotate file sink logger
 * 
 */
class logger : public std::enable_shared_from_this<logger>{
public:
    static std::shared_ptr<logger> create_defaultLogger();
    static std::shared_ptr<logger> create_rotateFileLogger(const std::string& logfilename);
    static std::shared_ptr<logger> create_asyncRotateFileLogger(const std::string& logfilename);
    static std::shared_ptr<logger> create_callBaclLogger(log_callback&& callback);
    static std::shared_ptr<logger> getLogger();
    void setLevel(log_level level);

public:
    logger()=default;
    ~logger()=default;

private:
    Z_DISABLE_COPY_MOVE(logger);

private:
    std::shared_ptr<spdlog::logger> spdlogger = nullptr;// spdlog::create<spdlog::rotating_logger_mt>()
};

};//!namespace zlog

#undef zlog_trace
#undef zlog_debug
#undef zlog_info
#undef zlog_warn
#undef zlog_error
#undef zlog
#ifndef zlog_trace
#define zlog_trace(...) SPDLOG_LOGGER_TRACE(spdlog::default_logger_raw(), __VA_ARGS__)
#endif
#ifndef zlog_debug
#define zlog_debug(...) SPDLOG_LOGGER_DEBUG(spdlog::default_logger_raw(), __VA_ARGS__)
#endif
#ifndef zlog_info
#define zlog_info(...) SPDLOG_LOGGER_INFO(spdlog::default_logger_raw(), __VA_ARGS__)
#endif
#ifndef zlog_warn
#define zlog_warn(...) SPDLOG_LOGGER_WARN(spdlog::default_logger_raw(), __VA_ARGS__)
#endif
#ifndef zlog_error
#define zlog_error(...) SPDLOG_LOGGER_ERROR(spdlog::default_logger_raw(), __VA_ARGS__)
#endif
#ifndef zlog
#define zlog(...) zlog_info(__VA_ARGS__)
#endif

#define LOG_INTERVAL_T(LEVEL,INTERVAL, ...) \
  constexpr std::chrono::nanoseconds LOG_TIME_PERIOD = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(INTERVAL)); \
  static std::atomic<int64_t> LOG_PREVIOUS_TIME_RAW; \
  const auto LOG_CURRENT_TIME = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()); \
  const auto LOG_PREVIOUS_TIME = LOG_PREVIOUS_TIME_RAW.load(std::memory_order_relaxed); \
  const auto LOG_TIME_DELTA = LOG_CURRENT_TIME - std::chrono::nanoseconds(LOG_PREVIOUS_TIME); \
  if (LOG_TIME_DELTA > LOG_TIME_PERIOD){ \
    LOG_PREVIOUS_TIME_RAW.store(std::chrono::duration_cast<std::chrono::nanoseconds>(LOG_CURRENT_TIME).count(), std::memory_order_relaxed); \
    zlog_ ## LEVEL(__VA_ARGS__); \
  }

#define LOG_COUNTER_C(LEVEL,COUNTER, ...) \
  static std::atomic<int> LOG_OCCURRENCES(0), LOG_OCCURRENCES_MOD_N(0); \
  ++LOG_OCCURRENCES; \
  if (++LOG_OCCURRENCES_MOD_N > COUNTER) LOG_OCCURRENCES_MOD_N -= COUNTER; \
  if (LOG_OCCURRENCES_MOD_N == 1){ \
    zlog_ ## LEVEL(__VA_ARGS__); \
  }

// INTERVAL use second
#define zlog_info_t(INTERVAL, ...) \
        LOG_INTERVAL_T(info,INTERVAL,__VA_ARGS__)

#define zlog_warn_t(INTERVAL, ...) \
        LOG_INTERVAL_T(warn,INTERVAL,__VA_ARGS__)

#define zlog_error_t(INTERVAL, ...) \
        LOG_INTERVAL_T(error,INTERVAL,__VA_ARGS__)

// COUNTER use int times 1,2,3...
#define zlog_info_c(COUNTER, ...) \
        LOG_COUNTER_C(info,COUNTER,__VA_ARGS__)

#define zlog_warn_c(COUNTER, ...) \
        LOG_COUNTER_C(warn,COUNTER,__VA_ARGS__)

#define zlog_error_c(COUNTER, ...) \
        LOG_COUNTER_C(error,COUNTER,__VA_ARGS__)

#endif //!ZLOG_LOG_H_