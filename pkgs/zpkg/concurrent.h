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
#ifndef ZPKG_CONCURRENT_H_
#define ZPKG_CONCURRENT_H_

#include <mutex>
#include <condition_variable>

namespace zpkg{

class block_oneshot{
public:
    block_oneshot()=default;
    ~block_oneshot()=default;

    template<typename BLOCK>
    void block(BLOCK&& block){
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock,[&](){
            return block();
        })
    }
    template<typename BLOCK>
    void block_timeout(BLOCK&& block,std::chrono::seconds sec){
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait_for(lock,sec,[&](){
            return block();
        })
    }

    template<typename SHOT>
    void one_shot(SHOT&& shot){
        shot();
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.notify_one();
    }

private:
std::mutex mutex_;
std::condition_variable cv_;

};

};//!namespace zpkg

#endif//!ZPKG_CONCURRENT_H_