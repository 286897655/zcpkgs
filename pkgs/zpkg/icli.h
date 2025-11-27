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

#ifndef ZPKG_ICLI_H_
#define ZPKG_ICLI_H_

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

namespace zpkg{

/**
 * interactive cli component
*/

class icli{
public:
    explicit icli(const std::string& prompt = "> ");
    ~icli() =default;

    using command_handler = std::function<void(const std::vector<std::string>& args)>;
    struct command_data
    {
        command_handler handler;
        std::string help;
    };
    
    void add_commmand(const std::string& command,const std::string& help,command_handler command_call);

    void interactive();

private:
    void show_help(const std::vector<std::string>& args);
    void show_all_help();
    void quit();

private:
    const std::string prompt_;
    std::unordered_map<std::string, command_data> commands_;
    bool running_;
    size_t max_command_length_;
    size_t max_help_legnth_;
};

};

#endif//!ICLI