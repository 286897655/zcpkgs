#pragma once

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

#ifndef ZPKG_SINGLETON_H_
#define ZPKG_SINGLETON_H_

#include <utility> //std::forward

namespace zpkg{

template<typename NoArgClass>
class NoArgSingleton{
public:
    static NoArgClass& Instance(){
        static NoArgClass noArgClass;
        return noArgClass;
    }
};

template<typename ArgsClass>
class ArgsSingleton{
public:
    template<typename ...Args>
    static ArgsClass* CreateInstance(Args &&... args){
        if(!instance_){
            instance_ = new ArgsClass(std::forward<Args>(args)...);
        }
        return instance_;
    }

    static ArgsClass* Instance(){
        return instance_;
    }
private:
    static ArgsClass* instance_;
};

template<typename ArgsClass>
ArgsClass* ArgsSingleton<ArgsClass>::instance_ = nullptr;

};//!namespace zpkg

#endif //!ZPKG_SINGLETON_H_